/** \file DIFdataExample.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#ifndef DIFDATAEXAMPLE_H_
#define DIFDATAEXAMPLE_H_

#include "Buffer.h"

class DIFdataExample
{
public:
  DIFdataExample();
  bool          nextEvent();
  bool          nextDIFbuffer();
  void getSDHCALBuffer(Buffer& buffer);

private:
  enum
  {
    BUFFER_SIZE = 143
  };  // 94+49=143
  //unsigned char _RAWbuffer[BUFFER_SIZE];
  //bool          _first;
  int           _stop{0};
  int           _stopDIF{0};
};

#endif  // DIFDATAEXAMPLE_H_
