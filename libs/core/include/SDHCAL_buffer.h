/** \file SDHCAL_buffer.h
 *  \copyright 2022 G.Grenier F.Lagarde A.Pingault L.Mirabito
 *  \see https://github.com/apingault/Trivent4HEP
 */

#pragma once

#include <iostream>
#include <utility>

class SDHCAL_buffer : public std::pair<unsigned char*, uint32_t>
{
public:
  SDHCAL_buffer(unsigned char* b, uint32_t i) : std::pair<unsigned char*, uint32_t>(b, i) { ; }
  unsigned char* buffer() { return first; }
  unsigned char* endOfBuffer() { return first + second; }
  uint32_t       getsize() { return second; }
  void           printBuffer(uint32_t start, uint32_t stop, std::ostream& flux = std::cout);
  void           printBuffer(uint32_t start = 0, std::ostream& flux = std::cout) { printBuffer(start, getsize()); }
};
