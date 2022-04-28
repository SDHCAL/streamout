/** \file SDHCAL_buffer_LoopCounter.cc
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#include "SDHCAL_buffer_LoopCounter.h"

void SDHCAL_buffer_LoopCounter::printAllCounters(std::ostream& out)
{
  out << "BUFFER LOOP FINAL STATISTICS : " << std::endl;
  printCounter("Start of DIF header", DIFStarter, out);
  printCounter("Value after DIF data are processed", DIFPtrValueAtReturnedPos, out);
  printCounter("Size remaining in buffer after end of DIF data", SizeAfterDIFPtr, out);
  out << "Number of Slow Control found " << hasSlowControl << " out of which " << hasBadSlowControl << " are bad" << std::endl;
  printCounter("Size remaining after all of data have been processed", SizeAfterAllData, out);
  printCounter("Number on non zero values in end of data buffer", NonZeroValusAtEndOfData, out);
}

void SDHCAL_buffer_LoopCounter::printCounter(const std::string& description, const std::map<int, int>& m, std::ostream& out)
{
  out << " statistics for " << description << " : ";
  for(std::map<int, int>::const_iterator it = m.begin(); it != m.end(); it++)
  {
    if(it != m.begin()) out << ",";
    out << " [" << it->first << "]=" << it->second;
  }
  out << std::endl;
}
