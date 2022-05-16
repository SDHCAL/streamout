/** \file DIFdataExample.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"

class DIFdataExample
{
public:
  DIFdataExample();
  bool          nextEvent();
  bool          nextDIFbuffer();
  Buffer getSDHCALBuffer() { return Buffer(_RAWbuffer, BUFFER_SIZE); }

private:
  enum
  {
    BUFFER_SIZE = 143
  };  // 94+49=143
  unsigned char _RAWbuffer[BUFFER_SIZE];
  bool          _first;
  int           _stop{0};
  int           _stopDIF{0};
};
