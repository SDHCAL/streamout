#ifndef _DIFSlowControl_H
#define _DIFSlowControl_H
#include <map>
#include <string>
#include <bitset>
#include <iostream>
#include <stdio.h>
#include "DIFUnpacker.h"
using namespace std;

/**
\class DIFSlowControl
  \author  L.Mirabito
  \date March 2010
  \version 1.0

 \brief Handler of DIF Slow Control  info

*/
class DIFSlowControl
{
public:
  //! Constructor
  /**
     @param version Data format version
     @param DIdi DIF id
     @param buf Pointer to the Raw data buffer
   */
  DIFSlowControl(unsigned int version,unsigned short DIdi,unsigned char *buf);

  //! Default Cosntructor
  DIFSlowControl(){}
  //! get DIF id
  unsigned short getDIFId() { return _DIFId;}

  //! Get chips map
  /**
     @return a map of < Asic Id, map of <string (parameter name),int (parameter value) >
   */
  map < int,map <string,int> > getChipsMap(){ return _mapSC;}

  //! Get one chip map
  /**
     @param asicid ASIC ID
     @return a map of <string (parameter name),int (parameter value) >
   */
  map<string,int> getChipSlowControl(int asicid){ return _mapSC[asicid];}

  //! Get one Chip value
  /**
     @param asicid ASic ID
     @param param Parameter name
   */
  int getChipSlowControl(int asicid,string param) { return getChipSlowControl(asicid)[param];}

  //! print out full map
  void Dump();
private:
  //! Fill hardROC 1 map
  void FillHR1(int header_shift,unsigned char *cbuf);
  //! Fill hardRoc 2 map
  void FillHR2(int header_shift,unsigned char *cbuf);
  //! read Asic HR1 type
  void FillAsicHR1(bitset<72*8> &bs);

  //! read Asic HR2 Type
  void FillAsicHR2(bitset<109*8> &bs);


  unsigned short			_DIFId; //! DIF Id
  unsigned int _version; //! version
  unsigned int _asicType;// asicType_
  unsigned int _nAsic; //! Number of Asic
  map< int,map < string,int > >        _mapSC; //! Storage map (asic,name,value)
};

class DIFPtr
{
public:
 DIFPtr(unsigned char* p,uint32_t max_size) : theDIF_(p),theSize_(max_size)
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
  inline uint32_t getThresholdStatus(uint32_t i,uint32_t ipad) { return  (((uint32_t) getFrameLevel(i,ipad,1))<<1) & ((uint32_t) getFrameLevel(i,ipad,0));}

 private:
  uint32_t theSize_;
  uint32_t theGetFramePtrReturn_;
  unsigned char* theDIF_;
  std::vector<unsigned char*> theFrames_;
  std::vector<unsigned char*> theLines_;
};
#endif
