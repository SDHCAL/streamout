/** \file Payload100.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once
#include "Payload.h"

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

class Payload100 : public Payload
{
public:
  Payload100() : Payload(100) {}
  bool                  hasTemperature() const;
  bool                  hasAnalogReadout() const;
  virtual std::uint32_t getNumberOfFrames() const final;
  virtual std::uint32_t getThresholdStatus(const std::uint32_t&, const std::uint32_t&) const final;
  virtual std::uint32_t getDIFid() const final;
  virtual std::uint32_t getDTC() const final;
  virtual std::uint32_t getGTC() const final;
  virtual std::uint32_t getBCID() const final;
  virtual std::uint64_t getAbsoluteBCID() const final;
  virtual std::uint32_t getASICid(const std::uint32_t&) const final;
  virtual std::uint32_t getFrameBCID(const std::uint32_t&) const final;
  virtual std::uint32_t getFrameTimeToTrigger(const std::uint32_t&) const final;

  /*
  bool hasAnalogReadout() const;

  bool hasSlowControl() const;

  float getTemperatureDIF() const;

  float getTemperatureASU1() const;

  float getTemperatureASU2() const;

  Buffer getSlowControl() const;

  std::vector<bit8_t*> getFramesVector() const;

  std::vector<bit8_t*> getLinesVector() const;

  bool          hasLine(const std::uint32_t&) const;

  bit8_t*       getFramePtr(const std::uint32_t&) const;

  std::uint32_t getDIF_CRC() const;

private:

  std::uint32_t getTASU1() const;
  std::uint32_t getTASU2() const;
  std::uint32_t getTDIF() const;
*/

private:
  bool                 getFrameLevel(const std::uint32_t&, const std::uint32_t&, const std::uint32_t&) const;
  std::uint16_t        m_Version{13};
  std::vector<bit8_t*> m_Lines;
  std::vector<bit8_t*> m_Frames;
  virtual void         parsePayload() final;
  std::uint32_t        parseAnalogLine(const std::uint32_t& idx);
  std::uint32_t        getNumberLines() const;
};
