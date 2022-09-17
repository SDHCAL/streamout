/** \file Payload150.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Payload150.h"

#include "Utilities.h"

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
  PMR_ID_SHIFT           = 1,
  PMR_NBASIC_SHIFT       = 1,
  PMR_FORMAT_SHIFT       = 1,
  PMR_GTC_SHIFT          = 3,
  PMR_ABCID_SHIFT        = 6,
  PMR_BCID_SHIFT         = 3,
  PMR_LTRG_SHIFT         = 4,
  //
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

static inline std::uint32_t operator+(const Size& a, const Size& b) { return static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b); }
static inline std::uint32_t operator+(const std::uint32_t& a, const Size& b) { return a + static_cast<std::uint32_t>(b); }
static inline std::uint32_t operator+(const Size& a) { return static_cast<std::uint32_t>(a); }

inline void Payload150::parsePayload()
{
  m_Frames.clear();
  std::uint32_t fshift{static_cast<std::uint32_t>(Size::GLOBAL_HEADER)};  // Pass Global Header
  // Pass DIF_ID, DIF Trigger counter, Information counter, Global Trigger counter, Absolute BCID, BCID DIF, NB line
  fshift += Size::PMR_ID_SHIFT + Size::PMR_NBASIC_SHIFT + Size::PMR_FORMAT_SHIFT + Size::PMR_GTC_SHIFT + Size::PMR_ABCID_SHIFT + Size::PMR_BCID_SHIFT + Size::PMR_LTRG_SHIFT;
  while(static_cast<std::uint8_t>(begin()[fshift]) != static_cast<std::uint8_t>(Value::GLOBAL_TRAILER))
  {
    m_Frames.push_back(&begin()[fshift]);
    fshift += Size::MICROROC_HEADER + Size::BCID + Size::DATA;
  }
  // Pass Global trailer
  fshift += +Size::GLOBAL_TRAILER;
  // Pass CRC MSB, CRC LSB
  fshift += Size::CRC_MSB + Size::CRC_LSB;
  theGetFramePtrReturn_ = fshift;
}

inline std::uint32_t Payload150::getNumberOfFrames() const { return m_Frames.size(); }

inline std::uint32_t Payload150::getThresholdStatus(const std::uint32_t& i, const std::uint32_t& ipad) const { return (((std::uint32_t)getFrameLevel(i, ipad, 1))) | ((std::uint32_t)getFrameLevel(i, ipad, 0) << 1); }

inline bool Payload150::getFrameLevel(const std::uint32_t& i, const std::uint32_t& ipad, const std::uint32_t& ilevel) const
{
  std::uint32_t shift{Size::MICROROC_HEADER + Size::BCID};
  return ((m_Frames[i][shift + ((3 - ipad / 16) * 4 + (ipad % 16) / 4)] >> (7 - (((ipad % 16) % 4) * 2 + ilevel))) & 0x1);
}

inline std::uint32_t Payload150::getDIFid() const
{
  std::uint32_t shift{+Size::GLOBAL_HEADER};
  return begin()[shift] & 0xFF;
}

inline std::uint32_t Payload150::getGTC() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::PMR_ID_SHIFT + Size::PMR_NBASIC_SHIFT + Size::PMR_FORMAT_SHIFT};
  return (begin()[shift] << 16) + (begin()[shift + 1] << 8) + begin()[shift + 2];
}

inline std::uint64_t Payload150::getAbsoluteBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::PMR_ID_SHIFT + Size::PMR_NBASIC_SHIFT + Size::PMR_FORMAT_SHIFT + Size::PMR_GTC_SHIFT};
  std::uint64_t LBC = ((begin()[shift] << 8) | (begin()[shift + 1])) * 16777216ULL + ((begin()[shift + 2] << 24) | (begin()[shift + 3] << 16) | (begin()[shift + 4] << 8) | begin()[shift + 5]);
  return LBC;
}

inline std::uint32_t Payload150::getDTC() const
{
  // MAYBE NOR USEFUL
  std::uint32_t shift{};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t Payload150::getBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::PMR_ID_SHIFT + Size::PMR_NBASIC_SHIFT + Size::PMR_FORMAT_SHIFT + Size::PMR_GTC_SHIFT+Size::PMR_ABCID_SHIFT};
  return (begin()[shift] << 16) + (begin()[shift + 1] << 8) + begin()[shift + 2];
}

inline std::uint32_t Payload150::getASICid(const std::uint32_t& i) const { return m_Frames[i][0] & 0xFF; }

inline std::uint32_t Payload150::getFrameBCID(const std::uint32_t& i) const
{
  std::uint32_t shift{+Size::MICROROC_HEADER};
  return GrayToBin((m_Frames[i][shift] << 16) + (m_Frames[i][shift + 1] << 8) + m_Frames[i][shift + 2]);
}

inline std::uint32_t Payload150::getFrameTimeToTrigger(const std::uint32_t& i) const { return getBCID() - getFrameBCID(i); }

Payload150::~Payload150() {}
