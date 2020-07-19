#pragma once
#include <vector>
#include "Constants.h"
#include <cstdint>
class DIFPtr
{
public:
  DIFPtr(unsigned char* p,const uint32_t& max_size,const std::uint32_t& idx=0);
  inline unsigned char* getPtr(){return theDIF_;}
  inline uint32_t getGetFramePtrReturn() {return theGetFramePtrReturn_;}
  inline std::vector<unsigned char*>& getFramesVector(){return theFrames_;}
  inline std::vector<unsigned char*>& getLinesVector(){return theLines_;}
  inline  uint32_t getID(const std::uint32_t& idx=0)
  {
    return theDIF_[idx+DU_ID_SHIFT];
  }
  inline  uint32_t getDTC(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_DTC_SHIFT]<<24)+(theDIF_[idx+DU_DTC_SHIFT+1]<<16)+(theDIF_[idx+DU_DTC_SHIFT+2]<<8)+theDIF_[idx+DU_DTC_SHIFT+3];;
  }
  inline  uint32_t getGTC(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_GTC_SHIFT]<<24)+(theDIF_[idx+DU_GTC_SHIFT+1]<<16)+(theDIF_[idx+DU_GTC_SHIFT+2]<<8)+theDIF_[idx+DU_GTC_SHIFT+3];
  }
  inline  unsigned long long getAbsoluteBCID(const std::uint32_t& idx=0)
  {
    unsigned long long Shift{16777216ULL};//to shift the value from the 24 first bits
    unsigned long long LBC= ( (theDIF_[idx+DU_ABCID_SHIFT]<<16) | (theDIF_[idx+DU_ABCID_SHIFT+1]<<8) | (theDIF_[idx+DU_ABCID_SHIFT+2]))*Shift+( (theDIF_[idx+DU_ABCID_SHIFT+3]<<16) | (theDIF_[idx+DU_ABCID_SHIFT+4]<<8) | (theDIF_[idx+DU_ABCID_SHIFT+5]));
    return LBC;
  }
  inline  uint32_t getBCID(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_BCID_SHIFT]<<16)+(theDIF_[idx+DU_BCID_SHIFT+1]<<8)+theDIF_[idx+DU_BCID_SHIFT+2];
  }
  inline  uint32_t getLines(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_LINES_SHIFT]>>4)& 0x5;
  }
  inline  bool hasLine(uint32_t line,const std::uint32_t& idx=0)
  {
    return ((theDIF_[idx+DU_LINES_SHIFT]>>line) & 0x1);
  }
  inline  uint32_t getTASU1(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_TASU1_SHIFT]<<24)+(theDIF_[idx+DU_TASU1_SHIFT+1]<<16)+(theDIF_[idx+DU_TASU1_SHIFT+2]<<8)+theDIF_[idx+DU_TASU1_SHIFT+3];
  }
  inline  uint32_t getTASU2(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_TASU2_SHIFT]<<24)+(theDIF_[idx+DU_TASU2_SHIFT+1]<<16)+(theDIF_[idx+DU_TASU2_SHIFT+2]<<8)+theDIF_[idx+DU_TASU2_SHIFT+3];
  }
  inline  uint32_t getTDIF(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx+DU_TDIF_SHIFT]);
  }
  inline  float getTemperatureDIF()
  {
    return 0.508*getTDIF()-9.659;
  }
  inline  float getTemperatureASU1()
  {
    return (getTASU1()>>3)*0.0625;
  }
  inline  float getTemperatureASU2()
  {
    return (getTASU2()>>3)*0.0625;
  }
  inline  bool hasTemperature(const std::uint32_t& idx=0)
  {
    return (theDIF_[idx]==DU_START_OF_DIF_TEMP);
  }
  inline  bool hasAnalogReadout(const std::uint32_t& idx=0)
  {
    return getLines(idx)!=0;
  }
  inline uint32_t getNumberOfFrames()
  {
    return theFrames_.size();
  }
  inline unsigned char* getFramePtr(uint32_t i)
  {
    return theFrames_[i];
  }
  inline uint32_t getFrameAsicHeader(uint32_t i)
  {
    return (theFrames_[i][DU_FRAME_ASIC_HEADER_SHIFT]);
  }
  inline uint32_t getFrameBCID(uint32_t i)
  {
    unsigned long long igray=(theFrames_[i][DU_FRAME_BCID_SHIFT]<<16)+(theFrames_[i][DU_FRAME_BCID_SHIFT+1]<<8)+theFrames_[i][DU_FRAME_BCID_SHIFT+2];
    return GrayToBin(igray);
  }
  inline uint32_t getFrameTimeToTrigger(uint32_t i)
  {
    return getBCID()-getFrameBCID(i);
  }
  inline bool getFrameLevel(uint32_t i,uint32_t ipad,uint32_t ilevel)
  {
    return ((theFrames_[i][DU_FRAME_DATA_SHIFT+((3-ipad/16)*4+(ipad%16)/4)]>>(7-(((ipad%16)%4)*2+ilevel))) & 0x1);
  }
  void dumpDIFInfo();
  //Addition by GG
  inline uint32_t getDIFid() 
  {
    return getID()&0xFF;
  }
  inline uint32_t getASICid(uint32_t i) 
  {
    return getFrameAsicHeader(i)&0xFF;
  }
  inline uint32_t getThresholdStatus(uint32_t i,uint32_t ipad) 
  { 
    return  (((uint32_t) getFrameLevel(i,ipad,1))<<1) | ((uint32_t) getFrameLevel(i,ipad,0));
  }
  
private:
  uint32_t theSize_;
  uint32_t theGetFramePtrReturn_;
  unsigned char* theDIF_{nullptr};
  std::vector<unsigned char*> theFrames_;
  std::vector<unsigned char*> theLines_;
  static unsigned long long GrayToBin(const unsigned long long& n);
};
