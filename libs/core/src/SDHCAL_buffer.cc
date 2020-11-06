#include "SDHCAL_buffer.h"

void SDHCAL_buffer::printBuffer(unsigned int start, unsigned int stop, std::ostream& flux)
{
  flux << std::hex;
  for(unsigned int k = start; k < stop; k++) flux << (unsigned int)(first[k]) << " - ";
  flux << std::dec << std::endl;
}
