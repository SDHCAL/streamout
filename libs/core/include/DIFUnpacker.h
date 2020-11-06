#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

class DIFUnpacker
{
public:
  static unsigned long long GrayToBin(const unsigned long long& n);
  static std::uint32_t      getStartOfDIF(const unsigned char* cbuf, const std::uint32_t& size_buf, const std::uint32_t& start = 92);
  static std::uint32_t      getID(const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getDTC(const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getGTC(const unsigned char* cb, const std::uint32_t& idx = 0);
  static unsigned long long getAbsoluteBCID(const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getBCID(const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getLines(const unsigned char* cb, const std::uint32_t& idx = 0);
  static bool               hasLine(const std::uint32_t& line, const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getTASU1(const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getTASU2(const unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t      getTDIF(const unsigned char* cb, const std::uint32_t& idx = 0);
  static bool               hasTemperature(const unsigned char* cb, const std::uint32_t& idx = 0);
  static bool               hasAnalogReadout(const unsigned char* cb, const std::uint32_t& idx = 0);

  static std::uint32_t getFrameAsicHeader(const unsigned char* framePtr);
  static std::uint32_t getFrameBCID(const unsigned char* framePtr);

  static bool getFramePAD(const unsigned char* framePtr, const std::uint32_t& ip);
  static bool getFrameLevel(const unsigned char* framePtr, const std::uint32_t& ip, const std::uint32_t& level);

  static std::uint32_t getAnalogPtr(std::vector<unsigned char*>& vLines, unsigned char* cb, const std::uint32_t& idx = 0);
  static std::uint32_t getFramePtr(std::vector<unsigned char*>& vFrame, std::vector<unsigned char*>& vLines, const std::uint32_t& max_size, unsigned char* cb, const std::uint32_t& idx = 0);
  static void          dumpFrameOld(const unsigned char* buf);
  static unsigned long swap_bytes(const unsigned int& n, const unsigned char* buf);  //Stolen from DCBufferReader
};
