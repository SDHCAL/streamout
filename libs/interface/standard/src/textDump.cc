#include "textDump.h"

#include <iostream>

void textDump::start() { _out << "Will dump bunch of DIF data" << std::endl; }

void textDump::processDIF(DIFPtr* d)
{
  if(NULL == d) return;
  _out << "DIF number is " << d->getDIFid() << std::endl;
  _out << "  DTC value is " << d->getDTC() << std::endl;
  _out << "  GTC value is " << d->getGTC() << std::endl;
  _out << "  DIF BCID is " << d->getBCID() << std::endl;
  _out << "  Absolute BCID is " << d->getAbsoluteBCID() << std::endl;
  _out << "  The number of frame is " << d->getNumberOfFrames() << std::endl;
}

void textDump::processFrame(DIFPtr* d, uint32_t frameIndex)
{
  _out << "  Displaying frame number " << frameIndex << std::endl;
  _out << "    ASIC ID is " << d->getASICid(frameIndex) << std::endl;
  _out << "    Frame BCID is " << d->getFrameBCID(frameIndex) << std::endl;
  _out << "    Frame Time To Trigger (a.k.a timestamp) is " << d->getFrameTimeToTrigger(frameIndex) << std::endl;
}

void textDump::processPadInFrame(DIFPtr* d, uint32_t frameIndex, uint32_t channelIndex)
{
  _out << "    Displaying channel number " << channelIndex << std::endl;
  _out << "      Threshold status is " << d->getThresholdStatus(frameIndex, channelIndex) << std::endl;
}

void textDump::processSlowControl(SDHCAL_buffer) { _out << "textDump::processSlowControl not implemented yet." << std::endl; }

void textDump::end() { _out << "textDump end of report" << std::endl; }
