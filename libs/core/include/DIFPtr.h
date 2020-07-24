#pragma once
#include <vector>
#include "DIFUnpacker.h"
#include <iostream>

class DIFPtr
{
public:
  DIFPtr(unsigned char* p,const uint32_t& max_size);
  inline unsigned char* getPtr(){return theDIF_;}
  inline uint32_t getGetFramePtrReturn() {return theGetFramePtrReturn_;}
  inline std::vector<unsigned char*>& getFramesVector(){return theFrames_;}
  inline std::vector<unsigned char*>& getLinesVector(){return theLines_;}
  inline  uint32_t getID(){return DIFUnpacker::getID(theDIF_);}
  inline  uint32_t getDTC(){return DIFUnpacker::getDTC(theDIF_);}
  inline  uint32_t getGTC(){return DIFUnpacker::getGTC(theDIF_);}
  inline  unsigned long long getAbsoluteBCID(){return DIFUnpacker::getAbsoluteBCID(theDIF_);}
  inline  uint32_t getBCID(){return DIFUnpacker::getBCID(theDIF_);}
  inline  uint32_t getLines(){return DIFUnpacker::getLines(theDIF_);}
  inline  bool hasLine(uint32_t line){return DIFUnpacker::hasLine(line,theDIF_);}
  inline  uint32_t getTASU1(){return DIFUnpacker::getTASU1(theDIF_);}
  inline  uint32_t getTASU2(){return DIFUnpacker::getTASU2(theDIF_);}
  inline  uint32_t getTDIF(){return DIFUnpacker::getTDIF(theDIF_);}
  inline  float getTemperatureDIF(){return 0.508*getTDIF()-9.659;}
  inline  float getTemperatureASU1(){return (getTASU1()>>3)*0.0625;}
  inline  float getTemperatureASU2(){return (getTASU2()>>3)*0.0625;}
  inline  bool hasTemperature(){return DIFUnpacker::hasTemperature(theDIF_);}
  inline  bool hasAnalogReadout(){return DIFUnpacker::hasAnalogReadout(theDIF_);}
  inline uint32_t getNumberOfFrames(){return theFrames_.size();}
  inline unsigned char* getFramePtr(uint32_t i){return theFrames_[i];}
  inline uint32_t getFrameAsicHeader(uint32_t i){return DIFUnpacker::getFrameAsicHeader(theFrames_[i]);}
  inline uint32_t getFrameBCID(uint32_t i){return DIFUnpacker::getFrameBCID(theFrames_[i]);}
  inline uint32_t getFrameTimeToTrigger(uint32_t i){return getBCID()-getFrameBCID(i);}
  inline bool getFrameLevel(uint32_t i,uint32_t ipad,uint32_t ilevel){return DIFUnpacker::getFrameLevel(theFrames_[i],ipad,ilevel);}
  void dumpDIFInfo()
  {
    printf("DIF %d DTC %d GTC %d ABCID %lld BCID %d Lines %d Temperature %d \n",
           getID(),
           getDTC(),
           getGTC(),
           getAbsoluteBCID(),
           getBCID(),
           getLines(),
           hasTemperature());

    if (hasTemperature())
      printf("T: ASU1 %d %f ASU2 %d %f DIF %d  %f \n",getTASU1(),getTemperatureASU1(),getTASU2(),getTemperatureASU2(),getTDIF(),getTemperatureDIF());
    printf("Found %ld Lines and %ld Frames \n",theLines_.size(),theFrames_.size());
  }
  //Addition by GG
  inline uint32_t getDIFid() {return getID()&0xFF;}
  inline uint32_t getASICid(uint32_t i) {return getFrameAsicHeader(i)&0xFF;}
  inline uint32_t getThresholdStatus(uint32_t i,uint32_t ipad) { return  (((uint32_t) getFrameLevel(i,ipad,1))<<1) | ((uint32_t) getFrameLevel(i,ipad,0));}

private:
  uint32_t theSize_;
  uint32_t theGetFramePtrReturn_;
  unsigned char* theDIF_;
  std::vector<unsigned char*> theFrames_;
  std::vector<unsigned char*> theLines_;
};
