#ifndef textDump_h
#define textDump_h

#include "SDHCAL_buffer.h"
#include "DIFPtr.h"
#include <iostream>
#include <ostream>

class textDump
{
 public:
  textDump(std::ostream &out=std::cout) : _out(out) {;}
  void start();
  void processDIF(DIFPtr*);
  void processFrame(DIFPtr*,uint32_t frameIndex);
  void processPadInFrame(DIFPtr*,uint32_t frameIndex, uint32_t channelIndex);
  void processSlowControl(SDHCAL_buffer);
  void end();
 private:
  std::ostream & _out;
};

#endif
