/** \file Words.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include <cstdint>

enum class Hardware : std::uint8_t
{
  NUMBER_PAD = 64,
};

enum class Size : std::uint8_t
{
  // Header
  DATA_FORMAT_VERSION    = 1,
  DAQ_SOFTWARE_VERSION   = 2,
  SDCC_FIRMWARE_VERSION  = 2,
  DIF_FIRMWARE_VERSION   = 2,
  TIMESTAMP_SECONDES     = 4,
  TIMESTAMP_MILLISECONDS = 4,
  // Payload
  GLOBAL_HEADER          = 1,
  DIF_IF                 = 1,
  DIF_TRIGGER_COUNTER    = 4,
  INFORMATION_COUNTER    = 4,
  GLOBAL_TRIGGER_COUNTER = 4,
  ABSOLUTE_BCID          = 6,
  BCID_DIF               = 3,
  NUMBER_LINE            = 1,
  TEMP_ASU1              = 4,
  TEMP_ASU2              = 4,
  TEMP_DIF               = 1,
  HEADER_LINE            = 1,
  NUMBER_CHIPS           = 1,
  LINE_SIZE              = 64 * 2,
  TRAILER_LINE           = 1,
  FRAME_HEADER           = 1,
  MICROROC_HEADER        = 1,
  BCID                   = 3,
  DATA                   = 16,
  FRAME_TRAILER          = 1,
  GLOBAL_TRAILER         = 1,
  CRC_MSB                = 1,
  CRC_LSB                = 1,
  // Slowcontrol
  SC_HEADER              = 1,
  DIF_ID                 = 1,
  ASIC_HEADER            = 1,
  SC_ASIC_SIZE           = 1,
  SC_TRAILER             = 1
};

static inline std::uint32_t operator+(const Size& a, const Size& b) { return static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b); }
static inline std::uint32_t operator+(const std::uint32_t& a, const Size& b) { return a + static_cast<std::uint32_t>(b); }
static inline std::uint32_t operator+(const Size& a) { return static_cast<std::uint32_t>(a); }

enum class Value : std::uint8_t
{
  GLOBAL_HEADER       = 0xb0,
  GLOBAL_HEADER_TEMP  = 0xbb,
  HEADER_LINE         = 0xc4,
  TRAILER_LINE        = 0xd4,
  FRAME_HEADER        = 0xb4,
  FRAME_TRAILER       = 0xa3,
  FRAME_TRAILER_ERROR = 0xc3,
  GLOBAL_TRAILER      = 0xa0,
  SC_HEADER           = 0xb1,
  SC_TRAILER          = 0xa1
};
