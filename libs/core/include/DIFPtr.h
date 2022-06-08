/** \file DIFPtr.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "DIFUnpacker.h"

#include <cstdint>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

class DIFPtr
{
public:
  void                         setBuffer(unsigned char* p, const std::uint32_t& max_size);
  unsigned char*               getPtr() const;
  std::uint32_t                getGetFramePtrReturn() const;
  std::vector<unsigned char*>& getFramesVector();
  std::vector<unsigned char*>& getLinesVector();
  std::uint32_t                getID() const;
  std::uint32_t                getDTC() const;
  std::uint32_t                getGTC() const;
  std::uint64_t                getAbsoluteBCID() const;
  std::uint32_t                getBCID() const;
  std::uint32_t                getLines() const;
  bool                         hasLine(uint32_t line) const;
  std::uint32_t                getTASU1() const;
  std::uint32_t                getTASU2() const;
  std::uint32_t                getTDIF() const;
  float                        getTemperatureDIF() const;
  float                        getTemperatureASU1() const;
  float                        getTemperatureASU2() const;
  bool                         hasTemperature() const;
  bool                         hasAnalogReadout() const;
  std::uint32_t                getNumberOfFrames() const;
  unsigned char*               getFramePtr(uint32_t i) const;
  std::uint32_t                getFrameAsicHeader(uint32_t i) const;
  std::uint32_t                getFrameBCID(uint32_t i) const;
  std::uint32_t                getFrameTimeToTrigger(uint32_t i) const;
  bool                         getFrameLevel(uint32_t i, uint32_t ipad, uint32_t ilevel) const;
  // Addition by GG
  uint32_t                     getDIFid() const;
  uint32_t                     getASICid(uint32_t i) const;
  uint32_t                     getThresholdStatus(uint32_t i, uint32_t ipad) const;

private:
  std::uint32_t               theSize_{0};
  std::uint32_t               theGetFramePtrReturn_{0};
  unsigned char*              theDIF_{nullptr};
  std::vector<unsigned char*> theFrames_;
  std::vector<unsigned char*> theLines_;
};

inline void DIFPtr::setBuffer(unsigned char* p, const std::uint32_t& max_size)
{
  theFrames_.clear();
  theLines_.clear();
  theSize_ = max_size;
  theDIF_  = p;
  try
  {
    theGetFramePtrReturn_ = DIFUnpacker::getFramePtr(theFrames_, theLines_, theSize_, theDIF_);
  }
  catch(const std::string& e)
  {
    spdlog::get("streamout")->error(" DIF {} T ? {} {}", getID(), hasTemperature(), e);
  }
}

inline unsigned char*               DIFPtr::getPtr() const { return theDIF_; }
inline std::uint32_t                DIFPtr::getGetFramePtrReturn() const { return theGetFramePtrReturn_; }
inline std::vector<unsigned char*>& DIFPtr::getFramesVector() { return theFrames_; }
inline std::vector<unsigned char*>& DIFPtr::getLinesVector() { return theLines_; }
inline std::uint32_t                DIFPtr::getID() const { return DIFUnpacker::getID(theDIF_); }
inline std::uint32_t                DIFPtr::getDTC() const { return DIFUnpacker::getDTC(theDIF_); }
inline std::uint32_t                DIFPtr::getGTC() const { return DIFUnpacker::getGTC(theDIF_); }
inline std::uint64_t                DIFPtr::getAbsoluteBCID() const { return DIFUnpacker::getAbsoluteBCID(theDIF_); }
inline std::uint32_t                DIFPtr::getBCID() const { return DIFUnpacker::getBCID(theDIF_); }
inline std::uint32_t                DIFPtr::getLines() const { return DIFUnpacker::getLines(theDIF_); }
inline bool                         DIFPtr::hasLine(uint32_t line) const { return DIFUnpacker::hasLine(line, theDIF_); }
inline std::uint32_t                DIFPtr::getTASU1() const { return DIFUnpacker::getTASU1(theDIF_); }
inline std::uint32_t                DIFPtr::getTASU2() const { return DIFUnpacker::getTASU2(theDIF_); }
inline std::uint32_t                DIFPtr::getTDIF() const { return DIFUnpacker::getTDIF(theDIF_); }
inline float                        DIFPtr::getTemperatureDIF() const { return 0.508 * getTDIF() - 9.659; }
inline float                        DIFPtr::getTemperatureASU1() const { return (getTASU1() >> 3) * 0.0625; }
inline float                        DIFPtr::getTemperatureASU2() const { return (getTASU2() >> 3) * 0.0625; }
inline bool                         DIFPtr::hasTemperature() const { return DIFUnpacker::hasTemperature(theDIF_); }
inline bool                         DIFPtr::hasAnalogReadout() const { return DIFUnpacker::hasAnalogReadout(theDIF_); }
inline std::uint32_t                DIFPtr::getNumberOfFrames() const { return theFrames_.size(); }
inline unsigned char*               DIFPtr::getFramePtr(uint32_t i) const { return theFrames_[i]; }
inline std::uint32_t                DIFPtr::getFrameAsicHeader(uint32_t i) const { return DIFUnpacker::getFrameAsicHeader(theFrames_[i]); }
inline std::uint32_t                DIFPtr::getFrameBCID(uint32_t i) const { return DIFUnpacker::getFrameBCID(theFrames_[i]); }
inline std::uint32_t                DIFPtr::getFrameTimeToTrigger(uint32_t i) const { return getBCID() - getFrameBCID(i); }
inline bool                         DIFPtr::getFrameLevel(uint32_t i, uint32_t ipad, uint32_t ilevel) const { return DIFUnpacker::getFrameLevel(theFrames_[i], ipad, ilevel); }
// Addition by GG
inline uint32_t                     DIFPtr::getDIFid() const { return getID() & 0xFF; }
inline uint32_t                     DIFPtr::getASICid(uint32_t i) const { return getFrameAsicHeader(i) & 0xFF; }
inline uint32_t                     DIFPtr::getThresholdStatus(uint32_t i, uint32_t ipad) const { return (((uint32_t)getFrameLevel(i, ipad, 1)) << 1) | ((uint32_t)getFrameLevel(i, ipad, 0)); }
