/** \file Interface.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "Buffer.h"

#include <memory>
#include <spdlog/logger.h>

class Interface
{
public:
  Interface() {}
  virtual ~Interface() {}
  std::shared_ptr<spdlog::logger>& log() { return m_Logger; }
  void                             setLogger(const std::shared_ptr<spdlog::logger>& logger) { m_Logger = logger; }

private:
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
};
