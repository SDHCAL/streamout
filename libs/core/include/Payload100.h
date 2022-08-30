/** \file Payload100.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Bits.h"
#include "Buffer.h"
#include "Exception.h"
#include "Formatters.h"
#include "Utilities.h"
#include "Words.h"

#include <cstdint>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

/** \class DIFPtr

M3 MICROROC and HARDROC2 dataformat

Data from the DAQ (once at the beginning of the file) :
    \image html FileHeader.png
    \image latex FileHeader.png "Data from the DAQ (once at the beginning of the file)"

Data from the DIF analog or/and digital (loop) :
    \image html DIF.png
    \image latex DIF.png "Data from the DIF analog or/and digital"

Data from the DAQ (slowcontrol) :
    \image html Slowcontrol.png
    \image latex Slowcontrol.png "Data from the DAQ (slowcontrol)"
*/

class Payload100 : public Buffer
{
public:
  Payload100() = default;

  void setBuffer(const Buffer& buffer);

  bool hasTemperature() const;

  bool hasAnalogReadout() const;

  bool hasSlowControl() const;

  float getTemperatureDIF() const;

  float getTemperatureASU1() const;

  float getTemperatureASU2() const;

  Buffer getSlowControl() const;

  std::vector<bit8_t*> getFramesVector() const;

  std::vector<bit8_t*> getLinesVector() const;

  std::uint32_t getSizeAfterDIFPtr() const;
  std::uint32_t getEndOfDIFData() const;
  std::uint32_t getDTC() const;
  std::uint32_t getGTC() const;
  std::uint64_t getAbsoluteBCID() const;
  std::uint32_t getBCID() const;
  bool          hasLine(const std::uint32_t&) const;
  std::uint32_t getNumberOfFrames() const;
  bit8_t*       getFramePtr(const std::uint32_t&) const;
  std::uint32_t getFrameBCID(const std::uint32_t&) const;
  std::uint32_t getFrameTimeToTrigger(const std::uint32_t&) const;
  bool          getFrameLevel(const std::uint32_t&, const std::uint32_t&, const std::uint32_t&) const;
  std::uint32_t getDIFid() const;
  std::uint32_t getASICid(const std::uint32_t&) const;
  std::uint32_t getThresholdStatus(const std::uint32_t&, const std::uint32_t&) const;
  std::uint32_t getDIF_CRC() const;

private:
  std::uint16_t m_Version{13};
  std::uint32_t parsePayload();
  std::uint32_t getNumberLines() const;
  std::uint32_t parseAnalogLine(const std::uint32_t& idx);
  std::uint32_t getTASU1() const;
  std::uint32_t getTASU2() const;
  std::uint32_t getTDIF() const;

  std::vector<bit8_t*> m_Lines;
  std::vector<bit8_t*> m_Frames;
  std::uint32_t        theGetFramePtrReturn_{0};
};

inline void Payload100::setBuffer(const Buffer& buffer)
{
  set(buffer);
  m_Frames.clear();
  m_Lines.clear();
  theGetFramePtrReturn_ = parsePayload();
}

inline std::uint32_t Payload100::parsePayload()
{
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
  return fshift;
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

inline std::uint64_t Payload100::getAbsoluteBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER};
  std::uint64_t LBC = ((begin()[shift] << 16) | (begin()[shift + 1] << 8) | (begin()[shift + 2])) * 16777216ULL /* to shift the value from the 24 first bits*/
                    + ((begin()[shift + 3] << 16) | (begin()[shift + 4] << 8) | (begin()[shift + 5]));
  return LBC;
}

inline std::uint32_t Payload100::getBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID};
  return (begin()[shift] << 16) + (begin()[shift + 1] << 8) + begin()[shift + 2];
}

inline bool Payload100::hasLine(const std::uint32_t& line) const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF};
  return ((begin()[shift] >> line) & 0x1);
}

inline std::uint32_t Payload100::getNumberOfFrames() const { return m_Frames.size(); }

inline bit8_t* Payload100::getFramePtr(const std::uint32_t& i) const { return m_Frames[i]; }

inline std::uint32_t Payload100::getFrameBCID(const std::uint32_t& i) const
{
  std::uint32_t shift{+Size::MICROROC_HEADER};
  return GrayToBin((m_Frames[i][shift] << 16) + (m_Frames[i][shift + 1] << 8) + m_Frames[i][shift + 2]);
}

inline std::uint32_t Payload100::getFrameTimeToTrigger(const std::uint32_t& i) const { return getBCID() - getFrameBCID(i); }

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

inline std::uint32_t Payload100::getASICid(const std::uint32_t& i) const { return m_Frames[i][0] & 0xFF; }

inline std::uint32_t Payload100::getThresholdStatus(const std::uint32_t& i, const std::uint32_t& ipad) const { return (((std::uint32_t)getFrameLevel(i, ipad, 1)) << 1) | ((std::uint32_t)getFrameLevel(i, ipad, 0)); }

inline std::uint32_t Payload100::getDIF_CRC() const
{
  std::uint32_t shift{getEndOfDIFData() - (Size::CRC_MSB + Size::CRC_LSB)};
  return (begin()[shift] << 8) + begin()[shift + 1];
}

inline std::uint32_t Payload100::getSizeAfterDIFPtr() const { return size() - theGetFramePtrReturn_; }

inline std::uint32_t Payload100::getEndOfDIFData() const { return theGetFramePtrReturn_; }
