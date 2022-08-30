/** \file RawBufferNavigator.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "RawBufferNavigator.h"

#include "Words.h"

int RawBufferNavigator::m_Start = 92;

void RawBufferNavigator::StartAt(const int& start)
{
  if(start >= 0) m_Start = start;
}

RawBufferNavigator::RawBufferNavigator() {}

void RawBufferNavigator::setBuffer(const Buffer& b)
{
  m_Buffer           = b;
  m_StartPayload     = -1;
  m_StartPayloadDone = false;
}

std::uint8_t RawBufferNavigator::getDetectorID() { return m_Buffer[0]; }

bool RawBufferNavigator::findStartOfPayload()
{
  if(m_StartPayloadDone == true)
  {
    if(m_StartPayload == -1) return false;
    else
      return true;
  }
  else
  {
    m_StartPayloadDone = true;
    for(std::size_t i = m_Start; i < m_Buffer.size(); i++)
    {
      if(static_cast<std::uint8_t>(m_Buffer[i]) == static_cast<std::uint8_t>(Valueksks::GLOBAL_HEADER) || static_cast<std::uint8_t>(m_Buffer[i]) == static_cast<std::uint8_t>(Valueksks::GLOBAL_HEADER_TEMP))
      {
        m_StartPayload = i;
        return true;
      }
    }
    m_StartPayload = -1;
    return false;
  }
}

std::int32_t RawBufferNavigator::getStartOfPayload()
{
  findStartOfPayload();
  return m_StartPayload;
}

bool RawBufferNavigator::validPayload() { return m_StartPayload != -1; }

Buffer RawBufferNavigator::getPayload() { return Buffer(&(m_Buffer.begin()[m_StartPayload]), m_Buffer.size() - m_StartPayload); }
