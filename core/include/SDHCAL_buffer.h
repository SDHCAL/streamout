//
// Code is extracted from Antoine Pingault Trivent4HEP
// see https://github.com/apingault/Trivent4HEP
//
// Code originally written by Laurent Mirabito
//

#pragma once

//#include <utility>
#include <iostream>
#include <assert.h>
#include "DIFSlowControl.h"
#include "DIFPtr.h"


class SDHCAL_buffer : public std::pair<unsigned char*, uint32_t>
{
 public:
  SDHCAL_buffer(unsigned char* b, uint32_t i) : std::pair<unsigned char*, uint32_t>(b,i) {;}
  unsigned char* buffer() {return first;}
  unsigned char* endOfBuffer() {return first+second;}
  uint32_t getsize() {return second;}
  void printBuffer(uint32_t start, uint32_t stop,std::ostream& flux=std::cout); 
  void printBuffer(uint32_t start=0,std::ostream& flux=std::cout) {printBuffer(start,getsize());}
};
