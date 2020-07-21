#pragma once

#include<map>
#include<string>
#include<iostream>

struct SDHCAL_buffer_LoopCounter
{
public:
  
  int hasSlowControl=0;
  int hasBadSlowControl=0;
  std::map<int,int> DIFStarter;
  std::map<int,int> DIFPtrValueAtReturnedPos;
  std::map<int,int> SizeAfterDIFPtr;
  std::map<int,int> SizeAfterAllData;
  std::map<int,int> NonZeroValusAtEndOfData;
  
  void printCounter(const std::string& description,const std::map<int,int> & m,std::ostream& out=std::cout);
  void printAllCounters(std::ostream& out=std::cout);
};
