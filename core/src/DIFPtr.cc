#include "DIFPtr.h"
#include "DIFUnpacker.h"
#include <iostream>

DIFPtr::DIFPtr(unsigned char* p,uint32_t max_size) : theDIF_(p),theSize_(max_size)
{
  theFrames_.clear();theLines_.clear();
  try
  {
    theGetFramePtrReturn_=DIFUnpacker::getFramePtr(theFrames_,theLines_,theSize_,theDIF_);
  }
  catch (std::string e)
  {
    std::cout<<"DIF "<<getID()<<" T ? "<<hasTemperature()<<" " <<e<<std::endl;
  }
}

inline unsigned char* DIFPtr::getPtr(){return theDIF_;}
inline std::uint32_t DIFPtr::getGetFramePtrReturn() {return theGetFramePtrReturn_;}
inline std::vector<unsigned char*>& DIFPtr::getFramesVector(){return theFrames_;}
inline std::vector<unsigned char*>& DIFPtr::getLinesVector(){return theLines_;}
inline std::uint32_t DIFPtr::getID(){return DIFUnpacker::getID(theDIF_);}
inline std::uint32_t DIFPtr::getDTC(){return DIFUnpacker::getDTC(theDIF_);}
inline std::uint32_t DIFPtr::getGTC(){return DIFUnpacker::getGTC(theDIF_);}
inline unsigned long long DIFPtr::getAbsoluteBCID(){return DIFUnpacker::getAbsoluteBCID(theDIF_);}
inline std::uint32_t DIFPtr::getBCID(){return DIFUnpacker::getBCID(theDIF_);}
inline std::uint32_t DIFPtr::getLines(){return DIFUnpacker::getLines(theDIF_);}
inline bool DIFPtr::hasLine(std::uint32_t line){return DIFUnpacker::hasLine(line,theDIF_);}
inline std::uint32_t DIFPtr::getTASU1(){return DIFUnpacker::getTASU1(theDIF_);}
inline std::uint32_t DIFPtr::getTASU2(){return DIFUnpacker::getTASU2(theDIF_);}
inline std::uint32_t DIFPtr::getTDIF(){return DIFUnpacker::getTDIF(theDIF_);}
inline float DIFPtr::getTemperatureDIF(){return 0.508*getTDIF()-9.659;}
inline float DIFPtr::getTemperatureASU1(){return (getTASU1()>>3)*0.0625;}
inline float DIFPtr::getTemperatureASU2(){return (getTASU2()>>3)*0.0625;}
inline bool DIFPtr::hasTemperature(){return DIFUnpacker::hasTemperature(theDIF_);}
inline bool DIFPtr::hasAnalogReadout(){return DIFUnpacker::hasAnalogReadout(theDIF_);}
inline std::uint32_t DIFPtr::getNumberOfFrames(){return theFrames_.size();}
inline unsigned char* DIFPtr::getFramePtr(std::uint32_t i){return theFrames_[i];}
inline std::uint32_t DIFPtr::getFrameAsicHeader(std::uint32_t i){return DIFUnpacker::getFrameAsicHeader(theFrames_[i]);}
inline std::uint32_t DIFPtr::getFrameBCID(std::uint32_t i){return DIFUnpacker::getFrameBCID(theFrames_[i]);}
inline std::uint32_t DIFPtr::getFrameTimeToTrigger(std::uint32_t i){return getBCID()-getFrameBCID(i);}
inline bool DIFPtr::getFrameLevel(std::uint32_t i,std::uint32_t ipad,std::uint32_t ilevel){return DIFUnpacker::getFrameLevel(theFrames_[i],ipad,ilevel);}
void DIFPtr::dumpDIFInfo()
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
inline std::uint32_t DIFPtr::getDIFid() {return getID()&0xFF;}
inline std::uint32_t DIFPtr::getASICid(std::uint32_t i) {return getFrameAsicHeader(i)&0xFF;}
inline std::uint32_t DIFPtr::getThresholdStatus(std::uint32_t i,std::uint32_t ipad) { return  (((std::uint32_t) getFrameLevel(i,ipad,1))<<1) | ((std::uint32_t) getFrameLevel(i,ipad,0));}
