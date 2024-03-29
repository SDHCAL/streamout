/** \file textDump.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Interface.h"
#include "Payload.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>
#include <spdlog/logger.h>

class textDump : public InterfaceWriter
{
public:
  textDump();
  void                             start(const VersionInfos& ver) final;
  void                             processDIF(const Payload&);
  void                             processFrame(const Payload&, uint32_t frameIndex);
  void                             processPadInFrame(const Payload&, uint32_t frameIndex, uint32_t channelIndex);
  void                             processSlowControl(Buffer);
  void                             end();
  std::shared_ptr<spdlog::logger>& print() { return m_InternalLogger; }
  void                             setLevel(const spdlog::level::level_enum& level) { m_InternalLogger->set_level(level); }

private:
  // This class is a dumb class to print on terminal so we need the logger + the standard one given by the interface.
  std::shared_ptr<spdlog::logger> m_InternalLogger{nullptr};
};
