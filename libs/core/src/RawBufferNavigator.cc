/** \file RawBufferNavigator.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "RawBufferNavigator.h"

#include "Words.h"
#include "spdlog/spdlog.h"

RawBufferNavigator::RawBufferNavigator(const std::shared_ptr<spdlog::logger>& logger) { m_Logger = logger; }

std::int32_t RawBufferNavigator::getStartOfPayload()
{
  for(std::size_t i = m_Start; i < m_Buffer.size(); i++)
  {
    if(m_Buffer[i] == DU::START_OF_DIF || m_Buffer[i] == DU::START_OF_DIF_TEMP) return i;
  }
  return -1;
}

int RawBufferNavigator::m_Start = 92;

void RawBufferNavigator::StartAt(const int& start)
{
  if(start >= 0) m_Start = start;
}

void RawBufferNavigator::setBuffer(const Buffer& b)
{
  m_BadSCdata     = false;
  m_Buffer        = b;
  m_DIFstartIndex = getStartOfPayload();
}

RawBufferNavigator::RawBufferNavigator(const Buffer& b) : m_Buffer(b) { setBuffer(b); }

std::uint8_t RawBufferNavigator::getDetectorID() { return m_Buffer[0]; }

bool RawBufferNavigator::validBuffer() { return m_DIFstartIndex != -1; }

std::int32_t RawBufferNavigator::getStartOfDIF() { return m_DIFstartIndex; }

unsigned char* RawBufferNavigator::getDIFBufferStart() { return &(m_Buffer.begin()[m_DIFstartIndex]); }

std::uint32_t RawBufferNavigator::getDIFBufferSize() { return m_Buffer.size() - m_DIFstartIndex; }

Buffer RawBufferNavigator::getDIFBuffer() { return Buffer(getDIFBufferStart(), getDIFBufferSize()); }

DIFPtr& RawBufferNavigator::getDIFPtr()
{
  m_TheDIFPtr.setBuffer(getDIFBufferStart(), getDIFBufferSize());
  return m_TheDIFPtr;
}

std::uint32_t RawBufferNavigator::getEndOfDIFData() { return getDIFPtr().getGetFramePtrReturn() + 3; }

std::uint32_t RawBufferNavigator::getSizeAfterDIFPtr() { return getDIFBufferSize() - getDIFPtr().getGetFramePtrReturn(); }

std::uint32_t RawBufferNavigator::getDIF_CRC()
{
  uint32_t i{getEndOfDIFData()};
  uint32_t ret{0};
  ret |= ((m_Buffer.begin()[i - 2]) << 8);
  ret |= m_Buffer.begin()[i - 1];
  return ret;
}

bool RawBufferNavigator::hasSlowControlData() { return getDIFBufferStart()[getEndOfDIFData()] == 0xb1; }

Buffer RawBufferNavigator::getSCBuffer()
{
  setSCBuffer();
  return m_SCbuffer;
}

bool RawBufferNavigator::badSCData()
{
  setSCBuffer();
  return m_BadSCdata;
}

void RawBufferNavigator::setSCBuffer()
{
  if(!hasSlowControlData()) return;
  if(m_SCbuffer.size() != 0) return;  // deja fait
  if(m_BadSCdata) return;
  m_SCbuffer.set(&(getDIFBufferStart()[getEndOfDIFData()]));
  // compute Slow Control size
  std::size_t maxsize{m_Buffer.size() - m_DIFstartIndex - getEndOfDIFData() + 1};  // should I +1 here ?
  uint32_t    k{1};                                                                // SC Header
  uint32_t    dif_ID{m_SCbuffer[1]};
  uint32_t    chipSize{m_SCbuffer[3]};
  while((dif_ID != 0xa1 && m_SCbuffer[k] != 0xa1 && k < maxsize) || (dif_ID == 0xa1 && m_SCbuffer[k + 2] == chipSize && k < maxsize))
  {
    k += 2;  // DIF ID + ASIC Header
    uint32_t scsize = m_SCbuffer[k];
    if(scsize != 74 && scsize != 109)
    {
      m_Logger->error("PROBLEM WITH SC SIZE {}", scsize);
      k           = 0;
      m_BadSCdata = true;
      break;
    }
    k++;          // skip size bit
    k += scsize;  // skip the data
  }
  if(m_SCbuffer[k] == 0xa1 && !m_BadSCdata) m_SCbuffer.setSize(k + 1);  // add the trailer
  else
  {
    m_BadSCdata = true;
    m_Logger->error("PROBLEM SC TRAILER NOT FOUND ");
  }
}

Buffer RawBufferNavigator::getEndOfAllData()
{
  setSCBuffer();
  if(hasSlowControlData() && !m_BadSCdata) { return Buffer(&(m_SCbuffer.begin()[m_SCbuffer.size()]), getSizeAfterDIFPtr() - 3 - m_SCbuffer.size()); }
  else
    return Buffer(&(getDIFBufferStart()[getEndOfDIFData()]), getSizeAfterDIFPtr() - 3);  // remove the 2 bytes for CRC and the DIF trailer
}
