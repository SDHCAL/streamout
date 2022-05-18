/** \file textDump.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "textDump.h"

#include "DIFPtr.h"

void textDump::start() { print()->info("Will dump bunch of DIF data"); }

void textDump::processDIF(const DIFPtr& d)
{
  print()->info("DIF number is {}", d.getDIFid());
  print()->info("DTC value is {}", d.getDTC());
  print()->info("GTC value is {}", d.getGTC());
  print()->info("DIF BCID is {}", d.getBCID());
  print()->info("Absolute BCID is {}", d.getAbsoluteBCID());
  print()->info("The number of frame is {}", d.getNumberOfFrames());
}

void textDump::processFrame(const DIFPtr& d, uint32_t frameIndex)
{
  print()->info("Displaying frame number {}", frameIndex);
  print()->info("ASIC ID is {}", d.getASICid(frameIndex));
  print()->info("Frame BCID is {}", d.getFrameBCID(frameIndex));
  print()->info("Frame Time To Trigger (a.k.a timestamp) is {}", d.getFrameTimeToTrigger(frameIndex));
}

void textDump::processPadInFrame(const DIFPtr& d, uint32_t frameIndex, uint32_t channelIndex)
{
  if(d.getThresholdStatus(frameIndex, channelIndex) > 0)
  {
    print()->info("Displaying channel number {}", channelIndex);
    print()->info("Threshold status is {}", d.getThresholdStatus(frameIndex, channelIndex));
  }
}

void textDump::processSlowControl(Buffer) { print()->error("textDump::processSlowControl not implemented yet."); }

void textDump::end() { print()->info("textDump end of report"); }
