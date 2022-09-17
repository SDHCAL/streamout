/**
*  \file Event.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "DIF.h"

#include <TObject.h>
#include <cstdint>
#include <map>

using DIFs_const_iterator = std::map<std::uint8_t, DIF>::const_iterator;

class Event : public TObject
{
public:
  std::uint32_t                               getEventNumber();
  void                                        setEventNumber(const std::uint32_t& evtNbr);
  void                                        clear();
  void                                        addDIF(const DIF& dif);
  std::map<std::uint8_t, DIF>::const_iterator cbegin() const;
  std::map<std::uint8_t, DIF>::const_iterator cend() const;

private:
  std::uint32_t m_EventNumber{0};
  std::map<std::uint8_t, DIF> DIFs;
  ClassDef(Event, 1);
};
