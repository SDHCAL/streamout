/**
*  \file SDHCAL_buffer.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Buffer.h"

void Buffer::printBuffer(unsigned int start, unsigned int stop, std::ostream& flux)
{
  flux << std::hex;
  for(unsigned int k = start; k < stop; k++) flux << (unsigned int)(m_Buffer[k]) << " - ";
  flux << std::dec << std::endl;
}

Buffer::~Buffer() {}
