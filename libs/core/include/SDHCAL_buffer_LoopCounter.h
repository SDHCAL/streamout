/** \file SDHCAL_buffer_LoopCounter.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include <map>
#include <memory>
#include <spdlog/fwd.h>
#include <string>

struct SDHCAL_buffer_LoopCounter
{
public:
  int                hasSlowControl    = 0;
  int                hasBadSlowControl = 0;
  std::map<int, int> DIFStarter;
  std::map<int, int> DIFPtrValueAtReturnedPos;
  std::map<int, int> SizeAfterDIFPtr;
  std::map<int, int> SizeAfterAllData;
  std::map<int, int> NonZeroValusAtEndOfData;

  void printCounter(const std::string& description, const std::map<int, int>& m, const std::shared_ptr<spdlog::logger>& logger);
  void printAllCounters(const std::shared_ptr<spdlog::logger>& logger);
};
