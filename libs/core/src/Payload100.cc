/** \file Payload100.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Payload100.h"

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

inline void Payload100::parsePayload()
{
  m_Frames.clear();
  m_Lines.clear();
  std::uint32_t fshift{static_cast<std::uint32_t>(Size::GLOBAL_HEADER)};  // Pass Global Header
  if(m_Version >= 13)
  {
    // Pass DIF_ID, DIF Trigger counter, Information counter, Global Trigger counter, Absolute BCID, BCID DIF, NB line
    fshift += Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE;
    // If has temperature infos then pass Temp ASU 1, Temp ASU 2, Temp DIF
    if(hasTemperature()) fshift += Size::TEMP_ASU1 + Size::TEMP_ASU2 + Size::TEMP_DIF;
    // If has AnalogReadout pass them
    if(hasAnalogReadout()) fshift = parseAnalogLine(fshift);  // to be implemented
  }
  else
    throw Exception(fmt::format("Version {} is not implemented", m_Version));

  while(static_cast<std::uint8_t>(begin()[fshift]) != static_cast<std::uint8_t>(Value::GLOBAL_TRAILER))
  {
    // If I found a FRAME_HEADER there is 2 cases :
    // 1) Nothing inside so FRAME_TRAILER comes just after
    // 2) Come MICROROC Header, BCID, DATA max 128 times
    if(static_cast<std::uint8_t>(begin()[fshift]) == static_cast<std::uint8_t>(Value::FRAME_HEADER))
    {
      fshift += +Size::FRAME_HEADER;
      if(static_cast<std::uint8_t>(begin()[fshift]) == static_cast<std::uint8_t>(Value::FRAME_TRAILER) || static_cast<std::uint8_t>(begin()[fshift]) == static_cast<std::uint8_t>(Value::FRAME_TRAILER_ERROR)) { fshift += +Size::FRAME_TRAILER; }
      else
      {
        while(static_cast<std::uint8_t>(begin()[fshift]) != static_cast<std::uint8_t>(Value::FRAME_TRAILER) && static_cast<std::uint8_t>(begin()[fshift]) != static_cast<std::uint8_t>(Value::FRAME_TRAILER_ERROR))
        {
          m_Frames.push_back(&begin()[fshift]);
          fshift += Size::MICROROC_HEADER + Size::BCID + Size::DATA;
        }
        fshift += +Size::FRAME_TRAILER;
      }
    }
  }
  // Pass Global trailer
  fshift += +Size::GLOBAL_TRAILER;
  // Pass CRC MSB, CRC LSB
  fshift += Size::CRC_MSB + Size::CRC_LSB;
  theGetFramePtrReturn_ = fshift;
}

inline bool Payload100::hasTemperature() const { return (static_cast<std::uint8_t>(begin()[0]) == static_cast<std::uint8_t>(Value::GLOBAL_HEADER_TEMP)); }

inline bool Payload100::hasAnalogReadout() const { return getNumberLines() != 0; }

inline std::uint32_t Payload100::getNumberLines() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF};
  return ((begin()[shift] >> 4) & 0x5);
}

inline std::uint32_t Payload100::parseAnalogLine(const std::uint32_t& idx)
{
  std::uint32_t fshift{idx};
  // Pass Header line
  if(static_cast<std::uint8_t>(begin()[fshift]) != static_cast<std::uint8_t>(Value::HEADER_LINE)) return fshift;
  else
    fshift += +Size::HEADER_LINE;
  while(static_cast<std::uint8_t>(begin()[fshift]) != static_cast<std::uint8_t>(Value::TRAILER_LINE))
  {
    m_Lines.push_back(&begin()[fshift]);
    // Get Number of CHIPS
    std::uint32_t nchip{begin()[fshift]};
    // Pass Number of CHIPS, NB Asicline*64*16bits
    fshift += +Size::NUMBER_CHIPS + static_cast<std::uint32_t>(Size::LINE_SIZE) * nchip;
  }
  // Pass Trailer line
  fshift += +Size::TRAILER_LINE;
  return fshift;
}

inline std::uint32_t Payload100::getNumberOfFrames() const { return m_Frames.size(); }

inline std::uint32_t Payload100::getThresholdStatus(const std::uint32_t& i, const std::uint32_t& ipad) const { return (((std::uint32_t)getFrameLevel(i, ipad, 1)) << 1) | ((std::uint32_t)getFrameLevel(i, ipad, 0)); }

inline bool Payload100::getFrameLevel(const std::uint32_t& i, const std::uint32_t& ipad, const std::uint32_t& ilevel) const
{
  std::uint32_t shift{Size::MICROROC_HEADER + Size::BCID};
  return ((m_Frames[i][shift + ((3 - ipad / 16) * 4 + (ipad % 16) / 4)] >> (7 - (((ipad % 16) % 4) * 2 + ilevel))) & 0x1);
}

inline std::uint32_t Payload100::getDIFid() const
{
  std::uint32_t shift{+Size::GLOBAL_HEADER};
  return begin()[shift] & 0xFF;
}

inline std::uint32_t Payload100::getDTC() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t Payload100::getGTC() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t Payload100::getBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID};
  return (begin()[shift] << 16) + (begin()[shift + 1] << 8) + begin()[shift + 2];
}

inline std::uint64_t Payload100::getAbsoluteBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER};
  std::uint64_t LBC = ((begin()[shift] << 16) | (begin()[shift + 1] << 8) | (begin()[shift + 2])) * 16777216ULL + ((begin()[shift + 3] << 16) | (begin()[shift + 4] << 8) | (begin()[shift + 5]));
  return LBC;
}

inline std::uint32_t Payload100::getASICid(const std::uint32_t& i) const { return m_Frames[i][0] & 0xFF; }

inline std::uint32_t Payload100::getFrameBCID(const std::uint32_t& i) const
{
  std::uint32_t shift{+Size::MICROROC_HEADER};
  return GrayToBin((m_Frames[i][shift] << 16) + (m_Frames[i][shift + 1] << 8) + m_Frames[i][shift + 2]);
}

inline std::uint32_t Payload100::getFrameTimeToTrigger(const std::uint32_t& i) const { return getBCID() - getFrameBCID(i); }

/*
inline bool Payload100::hasSlowControl() const { return theGetFramePtrReturn_ != size(); }

inline std::uint32_t Payload100::getTASU1() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t Payload100::getTASU2() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE + Size::TEMP_ASU1};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t Payload100::getTDIF() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE + Size::TEMP_ASU1 + Size::TEMP_ASU2};
  return begin()[shift];
}

inline float Payload100::getTemperatureDIF() const
{
  if(!hasTemperature()) throw Exception("Don't have TemperatureDIF information");
  return 0.508 * getTDIF() - 9.659;
}

inline float Payload100::getTemperatureASU1() const
{
  if(!hasTemperature()) throw Exception("Don't have TemperatureASU1 information");
  return (getTASU1() >> 3) * 0.0625;
}

inline float Payload100::getTemperatureASU2() const
{
  if(!hasTemperature()) throw Exception("Don't have TemperatureASU2 information");
  return (getTASU2() >> 3) * 0.0625;
}

inline Buffer Payload100::getSlowControl() const
{
  if(hasSlowControl()) return Buffer(&begin()[getEndOfDIFData()], size() - getEndOfDIFData());
  else
    return Buffer();
}

inline std::vector<bit8_t*> Payload100::getFramesVector() const { return m_Frames; }

inline std::vector<bit8_t*> Payload100::getLinesVector() const { return m_Lines; }

inline bool Payload100::hasLine(const std::uint32_t& line) const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF};
  return ((begin()[shift] >> line) & 0x1);
}

inline bit8_t* Payload100::getFramePtr(const std::uint32_t& i) const { return m_Frames[i]; }

inline std::uint32_t Payload100::getDIF_CRC() const
{
  std::uint32_t shift{getEndOfDIFData() - (Size::CRC_MSB + Size::CRC_LSB)};
  return (begin()[shift] << 8) + begin()[shift + 1];
}

 */
