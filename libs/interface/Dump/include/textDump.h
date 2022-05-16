/** \file textDump.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "DIFPtr.h"
#include "Buffer.h"

#include <iostream>
#include <ostream>

class textDump
{
public:
  explicit textDump(std::ostream& out = std::cout) : _out(out) { ; }
  void start();
  void processDIF(DIFPtr*);
  void processFrame(DIFPtr*, uint32_t frameIndex);
  void processPadInFrame(DIFPtr*, uint32_t frameIndex, uint32_t channelIndex);
  void processSlowControl(Buffer);
  void end();

private:
  std::ostream& _out;
};
