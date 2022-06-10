/** \file Interface.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "Buffer.h"

#include <memory>
#include <spdlog/logger.h>

/**
* template class should implement
* void SOURCE::start();
* bool SOURCE::next();
* void SOURCE::end();
* const Buffer& SOURCE::getSDHCALBuffer();
*
* void DESTINATION::begin();
* void DESTINATION::processDIF(const DIFPtr&);
* void DESTINATION::processFrame(const DIFPtr&,const std::uint32_t& frameIndex);
* void DESTINATION::processPadInFrame(const DIFPtr&,const std::uint32_t& frameIndex,const std::uint32_t& channelIndex);
* void DESTINATION::processSlowControl(const Buffer&);
* void DESTINATION::end();
**/

class Interface
{
public:
  Interface() {}
  virtual ~Interface() {}
  virtual void                     startEvent() {}
  virtual void                     endEvent() {}
  virtual void                     startDIF() {}
  virtual void                     endDIF() {}
  virtual void                     startFrame() {}
  virtual void                     endFrame() {}
  virtual void                     startPad() {}
  virtual void                     endPad() {}
  std::shared_ptr<spdlog::logger>& log() { return m_Logger; }
  void                             setLogger(const std::shared_ptr<spdlog::logger>& logger) { m_Logger = logger; }

private:
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
};
