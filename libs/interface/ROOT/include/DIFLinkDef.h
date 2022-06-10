/**
*  \file DIFLinkDef.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once
#include <vector>

#ifdef __CLING__
  #pragma link C++ class DIF;
  #pragma link C++ class Hit;
  #pragma link C++ class std::vector < Hit>;
#endif
