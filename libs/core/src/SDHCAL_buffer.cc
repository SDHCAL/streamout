#include "SDHCAL_buffer.h"

void SDHCAL_buffer::printBuffer(unsigned int start, unsigned int stop, std::ostream& flux)
{
  flux << std::hex;
  for(unsigned int k = start; k < stop; k++) flux << (unsigned int)(m_Buffer[k]) << " - ";
  flux << std::dec << std::endl;
}

SDHCAL_buffer::~SDHCAL_buffer()
{
  std::cout<<"SDHCAL_buffer destructor called"<<std::endl;
}
