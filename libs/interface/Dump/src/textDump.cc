/** \file textDump.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "textDump.h"

textDump::textDump() : InterfaceWriter("textDump", "1.0.0")
{
  m_InternalLogger = std::make_shared<spdlog::logger>("textDump", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  m_InternalLogger->set_level(spdlog::level::trace);
  addCompatibility("RawdataReader", ">=1.0.0");
  addCompatibility("DIFdataExample", ">=1.0.0");
}

void textDump::start(const VersionInfos& ver) { print()->info("Will dump bunch of DIF data"); }

void textDump::processDIF(const Payload& d) { print()->info("DIF_ID : {}, DTC : {}, GTC : {}, DIF BCID {}, Absolute BCID : {}, Nbr frames {}", d.getDIFid(), d.getDTC(), d.getGTC(), d.getBCID(), d.getAbsoluteBCID(), d.getNumberOfFrames()); }

void textDump::processFrame(const Payload& d, uint32_t frameIndex)
{
  print()->info("\tDisplaying frame number {} : ASIC ID {}, Frame BCID {}, Frame Time To Trigger (a.k.a timestamp) is {}", frameIndex, d.getASICid(frameIndex), d.getFrameBCID(frameIndex), d.getFrameTimeToTrigger(frameIndex));
}

void textDump::processPadInFrame(const Payload& d, uint32_t frameIndex, uint32_t channelIndex)
{
  if(d.getThresholdStatus(frameIndex, channelIndex) > 0) { print()->info("\t\tChannel {}, Threshold {}", channelIndex, d.getThresholdStatus(frameIndex, channelIndex)); }
}

void textDump::processSlowControl(Buffer) { print()->error("textDump::processSlowControl not implemented yet."); }

void textDump::end() { print()->info("textDump end of report"); }
