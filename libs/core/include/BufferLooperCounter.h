/** \file BufferLooperCounter.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include <ios>
#include <map>
#include <memory>
#include <string>

struct BufferLooperCounter
{
public:
  int                hasSlowControl    = 0;
  int                hasBadSlowControl = 0;
  std::map<int, int> DIFStarter;
  std::map<int, int> DIFPtrValueAtReturnedPos;
  std::map<int, int> SizeAfterDIFPtr;
  std::map<int, int> SizeAfterAllData;
  std::map<int, int> NonZeroValusAtEndOfData;

  void printCounter(const std::string& description, const std::map<int, int>& m, const std::ios_base::fmtflags& base = std::ios_base::dec);
  void printAllCounters();
};
