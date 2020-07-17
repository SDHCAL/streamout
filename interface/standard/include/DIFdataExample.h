#ifndef DIFdataExample_h
#define DIFdataExample_h

#include "SDHCAL_buffer.h"

class DIFdataExample
{
 public:
  DIFdataExample();
  bool next();
  SDHCAL_buffer getSDHCALBuffer() {return SDHCAL_buffer(_RAWbuffer, 60);}
 private:
  unsigned char _RAWbuffer[60];
  bool _first;
};

#endif
