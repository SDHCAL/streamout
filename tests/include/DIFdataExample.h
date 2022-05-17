/** \file DIFdataExample.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"
#include "Interface.h"

class DIFdataExample : public Interface
{
public:
  DIFdataExample();
  void   start() {}
  void   end() {}
  bool   nextEvent();
  bool   nextDIFbuffer();
  Buffer getSDHCALBuffer() { return Buffer(_RAWbuffer); }

private:
  static constexpr std::size_t    BUFFER_SIZE{143};  // 94+49=143
  std::array<bit8_t, BUFFER_SIZE> _RAWbuffer;
  bool                            _first;
  int                             _stop{0};
  int                             _stopDIF{0};
};
