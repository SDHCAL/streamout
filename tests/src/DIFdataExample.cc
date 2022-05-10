#include "DIFdataExample.h"

#include <iostream>
#include "Formatters.h"

DIFdataExample::DIFdataExample()
{

}

void DIFdataExample::getSDHCALBuffer(Buffer& buffer)
{
  buffer.allocate(BUFFER_SIZE);
  for(int i = 0; i < BUFFER_SIZE; ++i) buffer.setValue(i,0);
  int istuff              = 94;    // should be greater than 92 for SDHCAL data, for may 2012, it was 94
  buffer.setValue(istuff,0xB0);  // start DIF data
  //buffer.setValue(istuff + 1,133);   // DIF ID 1 byte
  buffer.setValue(istuff + 5, 1);     // DTC    4 bytes
  buffer.setValue(istuff + 9, 1);     // Information Counter 4 bytes
  buffer.setValue(istuff + 13, 1);     // GTC    4 bytes
  buffer.setValue(istuff + 19, 1);     // Absolute BCID 6 bytes
  buffer.setValue(istuff + 22, 1);     // DIF BCID
  buffer.setValue(istuff + 23, 0);     // number of analog lines
  buffer.setValue(istuff + 24, 0xB4);  // start digital RO info (frame header)
  // a frame is on 20 bytes
  buffer.setValue(istuff + 25, 3);     // ASIC ID ? 1 byte
  buffer.setValue(istuff + 28, 1);     // ASIC BCID (3 bytes)
  buffer.setValue(istuff + 33, 0x27);  // put some threshold crossed among 128 bits (16 bytes)
  buffer.setValue(istuff + 45, 0xA3);  // end  digital RO info
  buffer.setValue(istuff + 46,0xA0);  // end DIF data
  buffer.setValue(95, 133 + _stopDIF);  // DIF ID 1 byte
  // 2 extra bytes for CRC value not implemented : total DIF buffer size is 49 bytes
  std::cout << buffer <<std::endl;
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
    ++_stopDIF;
    return true;
  }
  _stopDIF = 0;
  return false;
}
