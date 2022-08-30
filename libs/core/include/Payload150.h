/** \file Payload150.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once
#include "Payload.h"

#include <vector>

class Payload150 : public Payload
{
public:
  Payload150() : Payload(150) {}
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
  virtual ~Payload150() {}

private:
  bool                 getFrameLevel(const std::uint32_t&, const std::uint32_t&, const std::uint32_t&) const;
  std::vector<bit8_t*> m_Frames;
  virtual void         parsePayload() final;
};
