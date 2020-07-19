#pragma once

#include <vector>
#include <cstdint>
#include <iostream>
class DIFUnpacker 
{
public:
  static unsigned long long GrayToBin(const unsigned long long& n);
  static std::uint32_t getStartOfDIF(const unsigned char* cbuf,const std::uint32_t& size_buf,const std::uint32_t& start=92);
  static std::uint32_t getID(const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getDTC(const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getGTC(const unsigned char* cb,const std::uint32_t& idx=0);
  static unsigned long long getAbsoluteBCID(const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getBCID(const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getLines(const unsigned char* cb,const std::uint32_t& idx=0);
  static bool hasLine(const std::uint32_t& line,const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getTASU1(const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getTASU2(const unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getTDIF(const unsigned char* cb,const std::uint32_t& idx=0);
  static bool hasTemperature(const unsigned char* cb,const std::uint32_t& idx=0);
  static bool hasAnalogReadout(const unsigned char* cb,const std::uint32_t& idx=0);

  static std::uint32_t getFrameAsicHeader(const unsigned char* framePtr);
  static std::uint32_t getFrameBCID(const unsigned char* framePtr);

  static bool getFramePAD(const unsigned char* framePtr,const std::uint32_t& ip);
  static bool getFrameLevel(const unsigned char* framePtr,const std::uint32_t& ip,const std::uint32_t& level);

  static std::uint32_t getAnalogPtr(std::vector<unsigned char*>& vLines,unsigned char* cb,const std::uint32_t& idx=0);
  static std::uint32_t getFramePtr(std::vector<unsigned char*>& vFrame,std::vector<unsigned char*>& vLines,const std::uint32_t& max_size,unsigned char* cb,const std::uint32_t& idx=0);
  static void dumpFrameOld(const unsigned char* buf);
  static unsigned long swap_bytes(const unsigned int& n,const unsigned char* buf); //Stolen from DCBufferReader
  
private:
  static constexpr int DU_DATA_FORMAT_VERSION{13};
  static constexpr int DU_START_OF_DIF{0xB0};
  static constexpr int DU_START_OF_DIF_TEMP{0xBB};
  static constexpr int DU_END_OF_DIF{0xA0};
  static constexpr int DU_START_OF_LINES{0xC4};
  static constexpr int DU_END_OF_LINES{0xD4};
  
  static constexpr int DU_START_OF_FRAME{0xB4};
  static constexpr int DU_END_OF_FRAME{0xA3};
  
  static constexpr int DU_ID_SHIFT{1};
  static constexpr int DU_DTC_SHIFT{2};
  static constexpr int DU_GTC_SHIFT{10};
  static constexpr int DU_ABCID_SHIFT{14};
  static constexpr int DU_BCID_SHIFT{20};
  static constexpr int DU_LINES_SHIFT{23};
  static constexpr int DU_TASU1_SHIFT{24};
  static constexpr int DU_TASU2_SHIFT{28};
  static constexpr int DU_TDIF_SHIFT{32};
  
  static constexpr int DU_FRAME_ASIC_HEADER_SHIFT{0};
  static constexpr int DU_FRAME_BCID_SHIFT{1};
  static constexpr int DU_FRAME_DATA_SHIFT{4};
  static constexpr int DU_FRAME_SIZE{20};
};
