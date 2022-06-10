/**
*  \file Event.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "DIF.h"

#include <TObject.h>
#include <cstdint>
#include <map>

class Event : public TObject
{
public:
  void clear();
  void addDIF(const DIF& dif);

private:
  std::map<std::uint8_t, DIF> DIFs;
  ClassDef(Event, 1);
};
