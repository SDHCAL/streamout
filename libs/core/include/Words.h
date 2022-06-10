/** \file Words.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include <cstdint>

enum DU : std::uint8_t
{
  START_OF_DIF      = 0xB0,
  START_OF_DIF_TEMP = 0xBB,
  END_OF_DIF        = 0xA0,
  START_OF_LINES    = 0xC4,
  END_OF_LINES      = 0xD4,

  START_OF_FRAME = 0xB4,
  END_OF_FRAME   = 0xA3,

  ID_SHIFT    = 1,
  DTC_SHIFT   = 2,
  GTC_SHIFT   = 10,
  ABCID_SHIFT = 14,
  BCID_SHIFT  = 20,
  LINES_SHIFT = 23,
  TASU1_SHIFT = 24,
  TASU2_SHIFT = 28,
  TDIF_SHIFT  = 32,

  FRAME_ASIC_HEADER_SHIFT = 0,
  FRAME_BCID_SHIFT        = 1,
  FRAME_DATA_SHIFT        = 4,
  FRAME_SIZE              = 20,

  NUMBER_PAD = 64
};
