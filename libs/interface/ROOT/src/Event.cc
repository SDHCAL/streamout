/**
*  \file Event.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Event.h"

std::uint32_t Event::getEventNumber()
{
  return m_EventNumber;
}

void Event::setEventNumber(const std::uint32_t& evtNbr)
{
  m_EventNumber=evtNbr;
}

void Event::clear() { DIFs.clear(); }

void Event::addDIF(const DIF& dif) { DIFs[dif.getID()] = dif; }

std::map<std::uint8_t, DIF>::const_iterator Event::cbegin() const { return DIFs.cbegin(); }

std::map<std::uint8_t, DIF>::const_iterator Event::cend() const { return DIFs.cend(); }
