/** \file SDHCAL_buffer_LoopCounter.cc
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#include "SDHCAL_buffer_LoopCounter.h"

#include <spdlog/spdlog.h>

void SDHCAL_buffer_LoopCounter::printAllCounters(const std::shared_ptr<spdlog::logger>& logger)
{
  spdlog::level::level_enum level = logger->level();
  logger->set_level(spdlog::level::trace);
  logger->critical("BUFFER LOOP FINAL STATISTICS : ");
  printCounter("Start of DIF header", DIFStarter, logger);
  printCounter("Value after DIF data are processed", DIFPtrValueAtReturnedPos, logger);
  printCounter("Size remaining in buffer after end of DIF data", SizeAfterDIFPtr, logger);
  logger->critical("Number of Slow Control found {}  out of which {} are bad", hasSlowControl, hasBadSlowControl);
  printCounter("Size remaining after all of data have been processed", SizeAfterAllData, logger);
  printCounter("Number on non zero values in end of data buffer", NonZeroValusAtEndOfData, logger);
  logger->set_level(level);
}

void SDHCAL_buffer_LoopCounter::printCounter(const std::string& description, const std::map<int, int>& m, const std::shared_ptr<spdlog::logger>& logger)
{
  std::string out{"statistics for " + description + " : "};
  for(std::map<int, int>::const_iterator it = m.begin(); it != m.end(); it++)
  {
    if(it != m.begin()) out += ",";
    out += " [" + std::to_string(it->first) + "]=" + std::to_string(it->second);
  }
  logger->critical(out);
}
