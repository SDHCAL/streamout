#pragma once

#include <vector>
#include <cstdint>
#include "Constants.h"
class DIFUnpacker 
{
public:





  static bool getFramePAD(const unsigned char* framePtr,const std::uint32_t& ip);

  static std::uint32_t getAnalogPtr(std::vector<unsigned char*>& vLines,unsigned char* cb,const std::uint32_t& idx=0);

  static void dumpFrameOld(const unsigned char* buf);
  static unsigned long swap_bytes(const unsigned int& n,const unsigned char* buf); //Stolen from DCBufferReader
  
};
