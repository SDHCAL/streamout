/**
*  \file Event.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Event.h"

void Event::clear() { DIFs.clear(); }

void Event::addDIF(const DIF& dif) { DIFs[dif.getID()] = dif; }
