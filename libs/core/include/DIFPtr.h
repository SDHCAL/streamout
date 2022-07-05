/** \file DIFPtr.h
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

class DIFPtr
{
public:
  DIFPtr() = default;

  void setBuffer(const Buffer& buffer)
  {
    setBuffer(buffer.begin(), buffer.size());
    m_BadSCdata = false;
  }

  bit8_t* begin() const { return theDIF_; }
  bit8_t* end() const { return theDIF_ + theSize_; }

  std::uint32_t getSizeAfterDIFPtr() { return theSize_ - getGetFramePtrReturn(); }
  bool          hasSlowControlData() { return theDIF_[getEndOfDIFData()] == 0xb1; }

  std::uint32_t getEndOfDIFData() { return getGetFramePtrReturn() + 3; }

  bool badSCData()
  {
    setSCBuffer();
    return m_BadSCdata;
  }

  void                  setBuffer(unsigned char*, const std::uint32_t&);
  bit8_t*               getPtr() const;
  std::uint32_t         getGetFramePtrReturn() const;
  std::vector<bit8_t*>& getFramesVector();
  std::vector<bit8_t*>& getLinesVector();
  std::uint32_t         getID() const;
  std::uint32_t         getDTC() const;
  std::uint32_t         getGTC() const;
  std::uint64_t         getAbsoluteBCID() const;
  std::uint32_t         getBCID() const;
  std::uint32_t         getLines() const;
  bool                  hasLine(const std::uint32_t&) const;
  std::uint32_t         getTASU1() const;
  std::uint32_t         getTASU2() const;
  std::uint32_t         getTDIF() const;
  float                 getTemperatureDIF() const;
  float                 getTemperatureASU1() const;
  float                 getTemperatureASU2() const;
  bool                  hasTemperature() const;
  bool                  hasAnalogReadout() const;
  std::uint32_t         getNumberOfFrames() const;
  bit8_t*               getFramePtr(const std::uint32_t&) const;
  std::uint32_t         getFrameAsicHeader(const std::uint32_t&) const;
  std::uint32_t         getFrameBCID(const std::uint32_t&) const;
  std::uint32_t         getFrameTimeToTrigger(const std::uint32_t&) const;
  bool                  getFrameLevel(const std::uint32_t&, const std::uint32_t&, const std::uint32_t&) const;
  // Addition by GG
  std::uint32_t         getDIFid() const;
  std::uint32_t         getASICid(const std::uint32_t&) const;
  std::uint32_t         getThresholdStatus(const std::uint32_t&, const std::uint32_t&) const;
  Buffer                getSCBuffer()
  {
    setSCBuffer();
    return m_SCbuffer;
  }
  Buffer getEndOfAllData()
  {
    setSCBuffer();
    if(hasSlowControlData() && !m_BadSCdata) { return Buffer(&(m_SCbuffer.begin()[m_SCbuffer.size()]), getSizeAfterDIFPtr() - 3 - m_SCbuffer.size()); }
    else
      return Buffer(&(theDIF_[getEndOfDIFData()]), getSizeAfterDIFPtr() - 3);  // remove the 2 bytes for CRC and the DIF trailer
  }
  std::uint32_t getDIF_CRC()
  {
    uint32_t i{getEndOfDIFData()};
    uint32_t ret{0};
    ret |= ((theDIF_[i - 2]) << 8);
    ret |= theDIF_[i - 1];
    return ret;
  }
  void setSCBuffer()
  {
    if(!hasSlowControlData()) return;
    if(m_SCbuffer.size() != 0) return;  // deja fait
    if(m_BadSCdata) return;
    m_SCbuffer.set(&(theDIF_[getEndOfDIFData()]));
    // compute Slow Control size
    std::size_t maxsize{theSize_ - getEndOfDIFData() + 1};  // should I +1 here ?
    uint32_t    k{1};                                       // SC Header
    uint32_t    dif_ID{m_SCbuffer[1]};
    uint32_t    chipSize{m_SCbuffer[3]};
    while((dif_ID != 0xa1 && m_SCbuffer[k] != 0xa1 && k < maxsize) || (dif_ID == 0xa1 && m_SCbuffer[k + 2] == chipSize && k < maxsize))
    {
      k += 2;  // DIF ID + ASIC Header
      uint32_t scsize = m_SCbuffer[k];
      if(scsize != 74 && scsize != 109)
      {
        k           = 0;
        m_BadSCdata = true;
        throw Exception(fmt::format("PROBLEM WITH SC SIZE {}", scsize));
      }
      k++;          // skip size bit
      k += scsize;  // skip the data
    }
    if(m_SCbuffer[k] == 0xa1 && !m_BadSCdata) m_SCbuffer.setSize(k + 1);  // add the trailer
    else
    {
      m_BadSCdata = true;
      throw Exception(fmt::format("PROBLEM SC TRAILER NOT FOUND "));
    }
  }

private:
  std::uint32_t        getAnalogPtr(const std::uint32_t& idx = 0);
  std::uint32_t        getFrameAsicHeaderInternal(const unsigned char* framePtr) const;
  std::uint32_t        getFramePtr();
  std::uint32_t        theSize_{0};
  std::uint32_t        theGetFramePtrReturn_{0};
  bit8_t*              theDIF_{nullptr};
  std::vector<bit8_t*> theFrames_;
  std::vector<bit8_t*> theLines_;
  bool                 m_BadSCdata{false};
  Buffer               m_SCbuffer;
};

inline std::uint32_t DIFPtr::getFrameAsicHeaderInternal(const bit8_t* framePtr) const { return (framePtr[DU::FRAME_ASIC_HEADER_SHIFT]); }

inline void DIFPtr::setBuffer(bit8_t* p, const std::uint32_t& max_size)
{
  theFrames_.clear();
  theLines_.clear();
  theSize_              = max_size;
  theDIF_               = p;
  theGetFramePtrReturn_ = getFramePtr();
}

inline bit8_t* DIFPtr::getPtr() const { return theDIF_; }

inline std::uint32_t DIFPtr::getGetFramePtrReturn() const { return theGetFramePtrReturn_; }

inline std::vector<bit8_t*>& DIFPtr::getFramesVector() { return theFrames_; }

inline std::vector<bit8_t*>& DIFPtr::getLinesVector() { return theLines_; }

inline std::uint32_t DIFPtr::getID() const { return theDIF_[DU::ID_SHIFT]; }

inline std::uint32_t DIFPtr::getDTC() const { return (theDIF_[DU::DTC_SHIFT] << 24) + (theDIF_[DU::DTC_SHIFT + 1] << 16) + (theDIF_[DU::DTC_SHIFT + 2] << 8) + theDIF_[DU::DTC_SHIFT + 3]; }

inline std::uint32_t DIFPtr::getGTC() const { return (theDIF_[DU::GTC_SHIFT] << 24) + (theDIF_[DU::GTC_SHIFT + 1] << 16) + (theDIF_[DU::GTC_SHIFT + 2] << 8) + theDIF_[DU::GTC_SHIFT + 3]; }

inline std::uint64_t DIFPtr::getAbsoluteBCID() const
{
  std::uint64_t LBC = ((theDIF_[DU::ABCID_SHIFT] << 16) | (theDIF_[DU::ABCID_SHIFT + 1] << 8) | (theDIF_[DU::ABCID_SHIFT + 2])) * 16777216ULL /* to shift the value from the 24 first bits*/
                    + ((theDIF_[DU::ABCID_SHIFT + 3] << 16) | (theDIF_[DU::ABCID_SHIFT + 4] << 8) | (theDIF_[DU::ABCID_SHIFT + 5]));
  return LBC;
}

inline std::uint32_t DIFPtr::getBCID() const { return (theDIF_[DU::BCID_SHIFT] << 16) + (theDIF_[DU::BCID_SHIFT + 1] << 8) + theDIF_[DU::BCID_SHIFT + 2]; }

inline std::uint32_t DIFPtr::getLines() const { return (theDIF_[DU::LINES_SHIFT] >> 4) & 0x5; }

inline bool DIFPtr::hasLine(const std::uint32_t& line) const { return ((theDIF_[DU::LINES_SHIFT] >> line) & 0x1); }

inline std::uint32_t DIFPtr::getTASU1() const { return (theDIF_[DU::TASU1_SHIFT] << 24) + (theDIF_[DU::TASU1_SHIFT + 1] << 16) + (theDIF_[DU::TASU1_SHIFT + 2] << 8) + theDIF_[DU::TASU1_SHIFT + 3]; }

inline std::uint32_t DIFPtr::getTASU2() const { return (theDIF_[DU::TASU2_SHIFT] << 24) + (theDIF_[DU::TASU2_SHIFT + 1] << 16) + (theDIF_[DU::TASU2_SHIFT + 2] << 8) + theDIF_[DU::TASU2_SHIFT + 3]; }

inline std::uint32_t DIFPtr::getTDIF() const { return theDIF_[DU::TDIF_SHIFT]; }

inline float DIFPtr::getTemperatureDIF() const { return 0.508 * getTDIF() - 9.659; }

inline float DIFPtr::getTemperatureASU1() const { return (getTASU1() >> 3) * 0.0625; }

inline float DIFPtr::getTemperatureASU2() const { return (getTASU2() >> 3) * 0.0625; }

inline bool DIFPtr::hasTemperature() const { return (theDIF_[0] == DU::START_OF_DIF_TEMP); }

inline bool DIFPtr::hasAnalogReadout() const { return getLines() != 0; }

inline std::uint32_t DIFPtr::getNumberOfFrames() const { return theFrames_.size(); }

inline bit8_t* DIFPtr::getFramePtr(const std::uint32_t& i) const { return theFrames_[i]; }

inline std::uint32_t DIFPtr::getFrameAsicHeader(const std::uint32_t& i) const { return getFrameAsicHeaderInternal(theFrames_[i]); }

inline std::uint32_t DIFPtr::getFrameBCID(const std::uint32_t& i) const { return GrayToBin((theFrames_[i][DU::FRAME_BCID_SHIFT] << 16) + (theFrames_[i][DU::FRAME_BCID_SHIFT + 1] << 8) + theFrames_[i][DU::FRAME_BCID_SHIFT + 2]); }

inline std::uint32_t DIFPtr::getFrameTimeToTrigger(const std::uint32_t& i) const { return getBCID() - getFrameBCID(i); }

inline bool DIFPtr::getFrameLevel(const std::uint32_t& i, const std::uint32_t& ipad, const std::uint32_t& ilevel) const
{
  return ((theFrames_[i][DU::FRAME_DATA_SHIFT + ((3 - ipad / 16) * 4 + (ipad % 16) / 4)] >> (7 - (((ipad % 16) % 4) * 2 + ilevel))) & 0x1);
}
// Addition by GG
inline uint32_t DIFPtr::getDIFid() const { return getID() & 0xFF; }

inline uint32_t DIFPtr::getASICid(const std::uint32_t& i) const { return getFrameAsicHeader(i) & 0xFF; }

inline uint32_t DIFPtr::getThresholdStatus(const std::uint32_t& i, const std::uint32_t& ipad) const { return (((std::uint32_t)getFrameLevel(i, ipad, 1)) << 1) | ((std::uint32_t)getFrameLevel(i, ipad, 0)); }

inline std::uint32_t DIFPtr::getFramePtr()
{
  std::uint32_t fshift{0};
  if(DATA_FORMAT_VERSION >= 13)
  {
    fshift = DU::LINES_SHIFT + 1;
    if(hasTemperature()) fshift = DU::TDIF_SHIFT + 1;      // jenlev 1
    if(hasAnalogReadout()) fshift = getAnalogPtr(fshift);  // to be implemented
  }
  else
    fshift = DU::BCID_SHIFT + 3;
  if(theDIF_[fshift] != DU::START_OF_FRAME) { throw Exception(fmt::format("This is not a start of frame {}", to_hex(theDIF_[fshift]))); }
  do {
    if(theDIF_[fshift] == DU::END_OF_DIF) return fshift;
    if(theDIF_[fshift] == DU::START_OF_FRAME) fshift++;
    if(theDIF_[fshift] == DU::END_OF_FRAME)
    {
      fshift++;
      continue;
    }
    std::uint32_t header = getFrameAsicHeaderInternal(&theDIF_[fshift]);
    if(header == DU::END_OF_FRAME) return (fshift + 2);
    if(header < 1 || header > 48) { throw Exception(fmt::format("{} Header problem {}", header, fshift)); }
    theFrames_.push_back(&theDIF_[fshift]);
    fshift += DU::FRAME_SIZE;
    if(fshift > theSize_) { throw Exception(fmt::format("fshift {} exceed {}", fshift, theSize_)); }
    if(theDIF_[fshift] == DU::END_OF_FRAME) fshift++;
  } while(true);
}

inline std::uint32_t DIFPtr::getAnalogPtr(const std::uint32_t& idx)
{
  std::uint32_t fshift{idx};
  if(theDIF_[fshift] != DU::START_OF_LINES) return fshift;
  fshift++;
  while(theDIF_[fshift] != DU::END_OF_LINES)
  {
    theLines_.push_back(&theDIF_[fshift]);
    std::uint32_t nchip{theDIF_[fshift]};
    fshift += 1 + nchip * 64 * 2;
  }
  return fshift++;
}
