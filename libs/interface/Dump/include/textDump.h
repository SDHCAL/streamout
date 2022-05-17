/** \file textDump.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "DIFPtr.h"
#include "Interface.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>
#include <ostream>
#include <spdlog/logger.h>

class textDump : public Interface
{
public:
  textDump()
  {
    m_InternalLogger = std::make_shared<spdlog::logger>("textDump", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    m_InternalLogger->set_level(spdlog::level::trace);
  }
  void                             start();
  void                             processDIF(DIFPtr*);
  void                             processFrame(DIFPtr*, uint32_t frameIndex);
  void                             processPadInFrame(DIFPtr*, uint32_t frameIndex, uint32_t channelIndex);
  void                             processSlowControl(Buffer);
  void                             end();
  std::shared_ptr<spdlog::logger>& print() { return m_InternalLogger; }
  void                             setLevel(const spdlog::level::level_enum& level) { m_InternalLogger->set_level(level); }

private:
  // This class is a dumb class to print on terminal so we need the logger + the standard one given by the interface.
  std::shared_ptr<spdlog::logger> m_InternalLogger{nullptr};
};
