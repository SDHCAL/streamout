/** \file DetectorId.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include <cstdint>

enum class DetectorID : std::uint16_t
{
  HARDROC     = 100,
  HARDROC_NEW = 150,
  RUNHEADER   = 255
};
