/** \file RawBufferNavigator.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "RawBufferNavigator.h"

std::int32_t RawBufferNavigator::m_StartPayload{20};

void RawBufferNavigator::startAt(const int& start)
{
  if(start >= 0) m_StartPayload = start;
}

RawBufferNavigator::RawBufferNavigator() {}

void RawBufferNavigator::setBuffer(const Buffer& b) { m_Buffer = b; }

std::uint32_t RawBufferNavigator::getDetectorID() { return m_Buffer[0]; }

std::int32_t RawBufferNavigator::getStartOfPayload() { return m_StartPayload; }

Buffer RawBufferNavigator::getPayload() { return Buffer(&(m_Buffer.begin()[m_StartPayload]), m_Buffer.size() - m_StartPayload); }
