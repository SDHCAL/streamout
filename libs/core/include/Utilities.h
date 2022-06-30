/** \file Utilities.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include <cstdint>

inline std::uint64_t GrayToBin(const std::uint64_t& n)
{
  std::uint64_t ish{1};
  std::uint64_t anss{n};
  std::uint64_t idiv{0};
  std::uint64_t ishmax{sizeof(std::uint64_t) * 8};
  while(true)
  {
    idiv = anss >> ish;
    anss ^= idiv;
    if(idiv <= 1 || ish == ishmax) return anss;
    ish <<= 1;
  }
}
