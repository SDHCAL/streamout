#include "DIFdataExample.h"

DIFdataExample::DIFdataExample(): _first(true)
{
  for(int i = 0; i < BUFFER_SIZE; ++i) _RAWbuffer[i] = 0;
  int istuff              = 94;    //should be greater than 92 for SDHCAL data, for may 2012, it was 94
  _RAWbuffer[istuff]      = 0xB0;  // start DIF data
  _RAWbuffer[istuff + 1]  = 133;   // DIF ID 1 byte
  _RAWbuffer[istuff + 5]  = 1;     // DTC    4 bytes
  _RAWbuffer[istuff + 9]  = 1;     // Information Counter 4 bytes
  _RAWbuffer[istuff + 13] = 1;     // GTC    4 bytes
  _RAWbuffer[istuff + 19] = 1;     // Absolute BCID 6 bytes
  _RAWbuffer[istuff + 22] = 1;     // DIF BCID
  _RAWbuffer[istuff + 23] = 0;     //number of analog lines
  _RAWbuffer[istuff + 24] = 0xB4;  //start digital RO info (frame header)
  // a frame is on 20 bytes
  _RAWbuffer[istuff + 25] = 3;     //ASIC ID ? 1 byte
  _RAWbuffer[istuff + 28] = 1;     //ASIC BCID (3 bytes)
  _RAWbuffer[istuff + 33] = 0x27;  // put some threshold crossed among 128 bits (16 bytes)
  _RAWbuffer[istuff + 45] = 0xA3;  // end  digital RO info
  _RAWbuffer[istuff + 46] = 0xA0;  // end DIF data
  // 2 extra bytes for CRC value not implemented : total DIF buffer size is 49 bytes
}

bool DIFdataExample::nextEvent()
{
  while(_stop != 10)
  {
    std::cout << "Event " << _stop << std::endl;
    _stop++;
    return true;
  }
  return false;
}

bool DIFdataExample::nextDIFbuffer()
{
  while(_stopDIF != 5)
  {
    _RAWbuffer[95] = 133 + _stopDIF;  // DIF ID 1 byte
    ++_stopDIF;
    return true;
  }
  _stopDIF = 0;
  return false;
}
