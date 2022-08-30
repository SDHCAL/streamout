/** \file Payload.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "Buffer.h"
#include "Exception.h"

#include <fmt/format.h>

class Payload : public Buffer
{
public:
  explicit Payload(const std::int32_t& detector_id) {}
  void                  setBuffer(const Buffer& buffer);
  std::uint32_t         getEndOfDIFData() const;
  std::uint32_t         getSizeAfterDIFPtr() const;
  virtual std::uint32_t getNumberOfFrames() const                                            = 0;
  virtual std::uint32_t getThresholdStatus(const std::uint32_t&, const std::uint32_t&) const = 0;
  virtual std::uint32_t getDIFid() const                                                     = 0;
  virtual std::uint32_t getDTC() const                                                       = 0;
  virtual std::uint32_t getGTC() const                                                       = 0;
  virtual std::uint32_t getBCID() const                                                      = 0;
  virtual std::uint64_t getAbsoluteBCID() const                                              = 0;
  virtual std::uint32_t getASICid(const std::uint32_t&) const                                = 0;
  virtual std::uint32_t getFrameBCID(const std::uint32_t&) const                             = 0;
  virtual std::uint32_t getFrameTimeToTrigger(const std::uint32_t&) const                    = 0;

  virtual ~Payload() {}

protected:
  virtual void  parsePayload() = 0;
  std::int32_t  m_DetectorID{-1};
  std::uint32_t theGetFramePtrReturn_{0};
};

inline void Payload::setBuffer(const Buffer& buffer)
{
  set(buffer);
  parsePayload();
}

inline std::uint32_t Payload::getEndOfDIFData() const { return theGetFramePtrReturn_; }

inline std::uint32_t Payload::getSizeAfterDIFPtr() const { return size() - theGetFramePtrReturn_; }
