/**
*  \file Event.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Event.h"

void Event::clear() { DIFs.clear(); }

void Event::addDIF(const DIF& dif) { DIFs[dif.getID()] = dif; }

std::map<std::uint8_t, DIF>::const_iterator Event::cbegin() const { return DIFs.cbegin(); }

std::map<std::uint8_t, DIF>::const_iterator Event::cend() const { return DIFs.cend(); }
