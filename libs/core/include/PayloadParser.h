/** \file PayloadParser.h
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

class PayloadParser : public Buffer
{
public:
  PayloadParser() = default;

  void setBuffer(const Buffer& buffer);

  bool hasTemperature() const;

  bool hasAnalogReadout() const;

  bool hasSlowControl() const;

  float getTemperatureDIF() const;

  float getTemperatureASU1() const;

  float getTemperatureASU2() const;

  Buffer getSlowControl() const;

  std::vector<bit8_t*>& getFramesVector();

  std::vector<bit8_t*>& getLinesVector();

  std::uint32_t getSizeAfterDIFPtr() { return size() - getGetFramePtrReturn(); }
  std::uint32_t getEndOfDIFData() const { return getGetFramePtrReturn() + 3; }
  bool          badSCData()
  {
    setSCBuffer();
    return m_BadSlowControl;
  }
  std::uint32_t getGetFramePtrReturn() const;
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
  // Addition by GG
  std::uint32_t getDIFid() const;
  std::uint32_t getASICid(const std::uint32_t&) const;
  std::uint32_t getThresholdStatus(const std::uint32_t&, const std::uint32_t&) const;
  Buffer        getSCBuffer()
  {
    setSCBuffer();
    return m_SCbuffer;
  }
  Buffer getEndOfAllData()
  {
    setSCBuffer();
    if(hasSlowControl() && !m_BadSlowControl) { return Buffer(&(m_SCbuffer.begin()[m_SCbuffer.size()]), getSizeAfterDIFPtr() - 3 - m_SCbuffer.size()); }
    else
      return Buffer(&(begin()[getEndOfDIFData()]), getSizeAfterDIFPtr() - 3);  // remove the 2 bytes for CRC and the DIF trailer
  }
  std::uint32_t getDIF_CRC()
  {
    uint32_t i{getEndOfDIFData()};
    uint32_t ret{0};
    ret |= ((begin()[i - 2]) << 8);
    ret |= begin()[i - 1];
    return ret;
  }
  void setSCBuffer()
  {
    if(!hasSlowControl()) return;
    if(m_SCbuffer.size() != 0) return;  // deja fait
    if(m_BadSlowControl) return;
    m_SCbuffer.set(&(begin()[getEndOfDIFData()]));
    // compute Slow Control size
    std::size_t maxsize{size() - getEndOfDIFData() + 1};  // should I +1 here ?
    uint32_t    k{1};                                     // SC Header
    uint32_t    dif_ID{m_SCbuffer[1]};
    uint32_t    chipSize{m_SCbuffer[3]};
    while((dif_ID != 0xa1 && m_SCbuffer[k] != 0xa1 && k < maxsize) || (dif_ID == 0xa1 && m_SCbuffer[k + 2] == chipSize && k < maxsize))
    {
      k += 2;  // DIF ID + ASIC Header
      uint32_t scsize = m_SCbuffer[k];
      if(scsize != 74 && scsize != 109)
      {
        k                = 0;
        m_BadSlowControl = true;
        throw Exception(fmt::format("PROBLEM WITH SC SIZE {}", scsize));
      }
      k++;          // skip size bit
      k += scsize;  // skip the data
    }
    if(m_SCbuffer[k] == 0xa1 && !m_BadSlowControl) m_SCbuffer.setSize(k + 1);  // add the trailer
    else
    {
      m_BadSlowControl = true;
      throw Exception(fmt::format("PROBLEM SC TRAILER NOT FOUND "));
    }
  }

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
  bool                 m_BadSlowControl{false};

  std::uint32_t theGetFramePtrReturn_{0};

  Buffer m_SCbuffer;
};

inline void PayloadParser::setBuffer(const Buffer& buffer)
{
  set(buffer);
  m_Frames.clear();
  m_Lines.clear();
  theGetFramePtrReturn_ = parsePayload();
  if(theGetFramePtrReturn_ != size()) { fmt::print("****************************\n"); }
  m_BadSlowControl = false;
}

inline std::uint32_t PayloadParser::parsePayload()
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

inline bool PayloadParser::hasTemperature() const { return (static_cast<std::uint8_t>(begin()[0]) == static_cast<std::uint8_t>(Value::GLOBAL_HEADER_TEMP)); }

inline bool PayloadParser::hasAnalogReadout() const { return getNumberLines() != 0; }

inline std::uint32_t PayloadParser::getNumberLines() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF};
  return ((begin()[shift] >> 4) & 0x5);
}

inline std::uint32_t PayloadParser::parseAnalogLine(const std::uint32_t& idx)
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

inline bool PayloadParser::hasSlowControl() const { return theGetFramePtrReturn_ != size(); }

inline std::uint32_t PayloadParser::getTASU1() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t PayloadParser::getTASU2() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE + Size::TEMP_ASU1};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t PayloadParser::getTDIF() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF + Size::NUMBER_LINE + Size::TEMP_ASU1 + Size::TEMP_ASU2};
  return begin()[shift];
}

inline float PayloadParser::getTemperatureDIF() const
{
  if(!hasTemperature()) throw Exception("Don't have TemperatureDIF information");
  return 0.508 * getTDIF() - 9.659;
}

inline float PayloadParser::getTemperatureASU1() const
{
  if(!hasTemperature()) throw Exception("Don't have TemperatureASU1 information");
  return (getTASU1() >> 3) * 0.0625;
}

inline float PayloadParser::getTemperatureASU2() const
{
  if(!hasTemperature()) throw Exception("Don't have TemperatureASU2 information");
  return (getTASU2() >> 3) * 0.0625;
}

inline Buffer PayloadParser::getSlowControl() const
{
  if(hasSlowControl()) return Buffer(&begin()[getEndOfDIFData()], size() - getEndOfDIFData());
  else
    return Buffer();
}

inline std::vector<bit8_t*>& PayloadParser::getFramesVector() { return m_Frames; }

inline std::vector<bit8_t*>& PayloadParser::getLinesVector() { return m_Lines; }

inline std::uint32_t PayloadParser::getGetFramePtrReturn() const { return theGetFramePtrReturn_ - 3; }

inline std::uint32_t PayloadParser::getDTC() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint32_t PayloadParser::getGTC() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER};
  return (begin()[shift] << 24) + (begin()[shift + 1] << 16) + (begin()[shift + 2] << 8) + begin()[shift + 3];
}

inline std::uint64_t PayloadParser::getAbsoluteBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER};
  std::uint64_t LBC = ((begin()[shift] << 16) | (begin()[shift + 1] << 8) | (begin()[shift + 2])) * 16777216ULL /* to shift the value from the 24 first bits*/
                    + ((begin()[shift + 3] << 16) | (begin()[shift + 4] << 8) | (begin()[shift + 5]));
  return LBC;
}

inline std::uint32_t PayloadParser::getBCID() const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID};
  return (begin()[shift] << 16) + (begin()[shift + 1] << 8) + begin()[shift + 2];
}

inline bool PayloadParser::hasLine(const std::uint32_t& line) const
{
  std::uint32_t shift{Size::GLOBAL_HEADER + Size::DIF_IF + Size::DIF_TRIGGER_COUNTER + Size::INFORMATION_COUNTER + Size::GLOBAL_TRIGGER_COUNTER + Size::ABSOLUTE_BCID + Size::BCID_DIF};
  return ((begin()[shift] >> line) & 0x1);
}

inline std::uint32_t PayloadParser::getNumberOfFrames() const { return m_Frames.size(); }

inline bit8_t* PayloadParser::getFramePtr(const std::uint32_t& i) const { return m_Frames[i]; }

inline std::uint32_t PayloadParser::getFrameBCID(const std::uint32_t& i) const
{
  std::uint32_t shift{+Size::MICROROC_HEADER};
  return GrayToBin((m_Frames[i][shift] << 16) + (m_Frames[i][shift + 1] << 8) + m_Frames[i][shift + 2]);
}

inline std::uint32_t PayloadParser::getFrameTimeToTrigger(const std::uint32_t& i) const { return getBCID() - getFrameBCID(i); }

inline bool PayloadParser::getFrameLevel(const std::uint32_t& i, const std::uint32_t& ipad, const std::uint32_t& ilevel) const
{
  std::uint32_t shift{Size::MICROROC_HEADER + Size::BCID};
  return ((m_Frames[i][shift + ((3 - ipad / 16) * 4 + (ipad % 16) / 4)] >> (7 - (((ipad % 16) % 4) * 2 + ilevel))) & 0x1);
}

inline uint32_t PayloadParser::getDIFid() const
{
  std::uint32_t shift{+Size::GLOBAL_HEADER};
  return begin()[shift] & 0xFF;
}

inline uint32_t PayloadParser::getASICid(const std::uint32_t& i) const { return m_Frames[i][0] & 0xFF; }

inline uint32_t PayloadParser::getThresholdStatus(const std::uint32_t& i, const std::uint32_t& ipad) const { return (((std::uint32_t)getFrameLevel(i, ipad, 1)) << 1) | ((std::uint32_t)getFrameLevel(i, ipad, 0)); }
