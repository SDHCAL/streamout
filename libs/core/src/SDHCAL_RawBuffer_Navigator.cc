/** \file SDHCAL_RawBuffer_Navigator.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "SDHCAL_RawBuffer_Navigator.h"

int SDHCAL_RawBuffer_Navigator::m_Start = 92;

void SDHCAL_RawBuffer_Navigator::StartAt(const int& start)
{
  if(start >= 0) m_Start = start;
}

SDHCAL_RawBuffer_Navigator::SDHCAL_RawBuffer_Navigator(const SDHCAL_buffer& b, const int& start) : m_Buffer(b), m_SCbuffer(0, 0)
{
  StartAt(start);
  m_DIFstartIndex = DIFUnpacker::getStartOfDIF(m_Buffer.begin(), m_Buffer.size(), m_Start);
}

SDHCAL_RawBuffer_Navigator::~SDHCAL_RawBuffer_Navigator()
{
  if(m_TheDIFPtr != nullptr) delete m_TheDIFPtr;
}

bool SDHCAL_RawBuffer_Navigator::validBuffer() { return m_DIFstartIndex != 0; }

std::uint32_t SDHCAL_RawBuffer_Navigator::getStartOfDIF() { return m_DIFstartIndex; }

unsigned char* SDHCAL_RawBuffer_Navigator::getDIFBufferStart() { return &(m_Buffer.begin()[m_DIFstartIndex]); }

std::uint32_t SDHCAL_RawBuffer_Navigator::getDIFBufferSize() { return m_Buffer.size() - m_DIFstartIndex; }

SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getDIFBuffer() { return SDHCAL_buffer(getDIFBufferStart(), getDIFBufferSize()); }

DIFPtr* SDHCAL_RawBuffer_Navigator::getDIFPtr()
{
  if(m_TheDIFPtr == nullptr) m_TheDIFPtr = new DIFPtr(getDIFBufferStart(), getDIFBufferSize());
  return m_TheDIFPtr;
}

std::uint32_t SDHCAL_RawBuffer_Navigator::getEndOfDIFData() { return getDIFPtr()->getGetFramePtrReturn() + 3; }

std::uint32_t SDHCAL_RawBuffer_Navigator::getSizeAfterDIFPtr() { return getDIFBufferSize() - getDIFPtr()->getGetFramePtrReturn(); }

uint32_t SDHCAL_RawBuffer_Navigator::getDIF_CRC()
{
  uint32_t i{getEndOfDIFData()};
  uint32_t ret{0};
  ret |= ((m_Buffer.begin()[i - 2]) << 8);
  ret |= m_Buffer.begin()[i - 1];
  return ret;
}

bool SDHCAL_RawBuffer_Navigator::hasSlowControlData() { return getDIFBufferStart()[getEndOfDIFData()] == 0xb1; }

SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getSCBuffer()
{
  setSCBuffer();
  return m_SCbuffer;
}

bool SDHCAL_RawBuffer_Navigator::badSCData()
{
  setSCBuffer();
  return m_BadSCdata;
}

void SDHCAL_RawBuffer_Navigator::setSCBuffer()
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
      std::cout << "PROBLEM WITH SC SIZE " << scsize << std::endl;
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
    std::cout << "PROBLEM SC TRAILER NOT FOUND " << std::endl;
  }
}

SDHCAL_buffer SDHCAL_RawBuffer_Navigator::getEndOfAllData()
{
  setSCBuffer();
  if(hasSlowControlData() && !m_BadSCdata) { return SDHCAL_buffer(&(m_SCbuffer.begin()[m_SCbuffer.size()]), getSizeAfterDIFPtr() - 3 - m_SCbuffer.size()); }
  else
    return SDHCAL_buffer(&(getDIFBufferStart()[getEndOfDIFData()]), getSizeAfterDIFPtr() - 3);  // remove the 2 bytes for CRC and the DIF trailer
}
