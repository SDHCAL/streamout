/**
*  \file EventLinkDef.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once
#include <cstdint>
#include <map>
#include <vector>
#ifdef __CLING__
  #pragma link C++ class DIF;
  #pragma link C++ class std::vector < DIF>;
  #pragma link C++ class Hit;
  #pragma link C++ class std::vector < Hit>;
  #pragma link C++ class Event;
  #pragma link C++ class std::vector < Event>;
  #pragma link C++ class std::map < std::uint8_t, DIF>;
#endif
