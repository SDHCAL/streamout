#pragma once

#include <cstdint>
#include <vector>
#include "Words.h"

class DIFPtr
{
public:
  DIFPtr(unsigned char* p,const std::uint32_t& max_size,const std::uint32_t shift=0);
  inline unsigned char* getPtr()
  {
    return m_DIF;
  }
  inline std::uint32_t getGetFramePtrReturn() 
  {
    return m_FramePtr;
  }
  inline std::vector<unsigned char*>& getFramesVector()
  {
    return m_Frames;
  }
  inline std::vector<unsigned char*>& getLinesVector()
  {
    return m_Lines;
  }
  inline  uint32_t getID()
  {
    return m_DIF[m_Shift+DU::ID_SHIFT];
  }
  inline  uint32_t getDTC()
  {
    return (m_DIF[m_Shift+DU::DTC_SHIFT]<<24)+(m_DIF[m_Shift+DU::DTC_SHIFT+1]<<16)+(m_DIF[m_Shift+DU::DTC_SHIFT+2]<<8)+m_DIF[m_Shift+DU::DTC_SHIFT+3];
  }
  inline  uint32_t getGTC()
  {
    return (m_DIF[m_Shift+DU::GTC_SHIFT]<<24)+(m_DIF[m_Shift+DU::GTC_SHIFT+1]<<16)+(m_DIF[m_Shift+DU::GTC_SHIFT+2]<<8)+m_DIF[m_Shift+DU::GTC_SHIFT+3];
  }
  inline  unsigned long long getAbsoluteBCID()
  {
    unsigned long long Shift{16777216ULL};//to shift the value from the 24 first bits
    unsigned long long LBC= ( (m_DIF[m_Shift+DU::ABCID_SHIFT]<<16) | (m_DIF[m_Shift+DU::ABCID_SHIFT+1]<<8) | (m_DIF[m_Shift+DU::ABCID_SHIFT+2]))*Shift+( (m_DIF[m_Shift+DU::ABCID_SHIFT+3]<<16) | (m_DIF[m_Shift+DU::ABCID_SHIFT+4]<<8) | (m_DIF[m_Shift+DU::ABCID_SHIFT+5]));
    return LBC;
  }
  inline  uint32_t getBCID()
  {
    return (m_DIF[m_Shift+DU::BCID_SHIFT]<<16)+(m_DIF[m_Shift+DU::BCID_SHIFT+1]<<8)+m_DIF[m_Shift+DU::BCID_SHIFT+2];
  }
  inline  uint32_t getLines()
  {
    return (m_DIF[m_Shift+DU::LINES_SHIFT]>>4)& 0x5;
  }
  inline  bool hasLine(uint32_t line)
  {
    return ((m_DIF[m_Shift+DU::LINES_SHIFT]>>line) & 0x1);
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
  inline  bool hasTemperature()
  {
    return (m_DIF[m_Shift]==DU::START_OF_DIF_TEMP);
  }
  inline  bool hasAnalogReadout()
  {
    return (getLines()!=0);
  }
  inline uint32_t getNumberOfFrames()
  {
    return m_Frames.size();
  }
  inline unsigned char* getFramePtr(uint32_t i)
  {
    return m_Frames[i];
  }
  inline uint32_t getFrameAsicHeader(uint32_t i)
  {
    return (&m_DIF[i])[DU::FRAME_ASIC_HEADER_SHIFT];
  }
  inline uint32_t getFrameBCID(uint32_t i)
  {
    unsigned long long igray=(m_Frames[i][DU::FRAME_BCID_SHIFT]<<16)+(m_Frames[i][DU::FRAME_BCID_SHIFT+1]<<8)+m_Frames[i][DU::FRAME_BCID_SHIFT+2];
    return GrayToBin(igray);
  }
  inline uint32_t getFrameTimeToTrigger(uint32_t i)
  {
    return getBCID()-getFrameBCID(i);
  }
  inline bool getFrameLevel(uint32_t i,uint32_t ipad,uint32_t ilevel)
  {
    return ((m_Frames[i][DU::FRAME_DATA_SHIFT+((3-ipad/16)*4+(ipad%16)/4)]>>(7-(((ipad%16)%4)*2+ilevel))) & 0x1);
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
  std::uint32_t getAnalogPtr();
  static unsigned long long GrayToBin(const unsigned long long& n);
  inline  uint32_t getTASU1()
  {
    return (m_DIF[m_Shift+DU::TASU1_SHIFT]<<24)+(m_DIF[m_Shift+DU::TASU1_SHIFT+1]<<16)+(m_DIF[m_Shift+DU::TASU1_SHIFT+2]<<8)+m_DIF[m_Shift+DU::TASU1_SHIFT+3];
  }
  inline  uint32_t getTASU2()
  {
    return (m_DIF[m_Shift+DU::TASU2_SHIFT]<<24)+(m_DIF[m_Shift+DU::TASU2_SHIFT+1]<<16)+(m_DIF[m_Shift+DU::TASU2_SHIFT+2]<<8)+m_DIF[m_Shift+DU::TASU2_SHIFT+3];
  }
  inline  uint32_t getTDIF()
  {
    return (m_DIF[m_Shift+DU::TDIF_SHIFT]);
  }
  std::uint32_t m_MaxSize{0};
  std::uint32_t m_FramePtr{0};
  std::uint32_t m_Shift{0};
  unsigned char* m_DIF{nullptr};
  std::vector<unsigned char*> m_Frames;
  std::vector<unsigned char*> m_Lines;
};

//static bool getFramePAD(const unsigned char* framePtr,const std::uint32_t& ip);

//static void dumpFrameOld(const unsigned char* buf);
//static unsigned long swap_bytes(const unsigned int& n,const unsigned char* buf); //Stolen from DCBufferReader
