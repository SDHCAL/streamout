#ifndef DIFdataExample_h
#define DIFdataExample_h

#include "SDHCAL_buffer.h"

class DIFdataExample
{
public:
  DIFdataExample();
  bool nextEvent();
  bool nextDIFbuffer();
  SDHCAL_buffer getSDHCALBuffer() {return SDHCAL_buffer(_RAWbuffer, BUFFER_SIZE);}
private:
  enum {BUFFER_SIZE=143};  //94+49=143
  unsigned char _RAWbuffer[BUFFER_SIZE]; 
  bool _first;
  int _stop{0};
  int _stopDIF{0};
};

#endif
