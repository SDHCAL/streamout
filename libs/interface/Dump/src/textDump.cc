/** \file textDump.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "textDump.h"

#include "DIFPtr.h"

void textDump::start() { print()->info("Will dump bunch of DIF data"); }

void textDump::processDIF(const DIFPtr& d) { print()->info("DIF_ID : {}, DTC : {}, GTC : {}, DIF BCID {}, Absolute BCID : {}, Nbr frames {}", d.getDIFid(), d.getDTC(), d.getGTC(), d.getBCID(), d.getAbsoluteBCID(), d.getNumberOfFrames()); }

void textDump::processFrame(const DIFPtr& d, uint32_t frameIndex)
{
  print()->info("\tDisplaying frame number {} : ASIC ID {}, Frame BCID {}, Frame Time To Trigger (a.k.a timestamp) is {}", frameIndex, d.getASICid(frameIndex), d.getFrameBCID(frameIndex), d.getFrameTimeToTrigger(frameIndex));
}

void textDump::processPadInFrame(const DIFPtr& d, uint32_t frameIndex, uint32_t channelIndex)
{
  if(d.getThresholdStatus(frameIndex, channelIndex) > 0) { print()->info("\t\tChannel {}, Threshold {}", channelIndex, d.getThresholdStatus(frameIndex, channelIndex)); }
}

void textDump::processSlowControl(Buffer) { print()->error("textDump::processSlowControl not implemented yet."); }

void textDump::end() { print()->info("textDump end of report"); }
