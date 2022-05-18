/** \file Formatters.test.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/
#include "Formatters.h"

#include "Bits.h"

#include <iostream>

int main()
{
  bit8_t  a{2};
  bit16_t b{2};
  bit32_t c{2};
  bit64_t d{2};
  std::cout << to_bin(a) << " " << to_dec(a) << " " << to_hex(a) << " " << to_oct(a) << std::endl;
  std::cout << to_bin(b) << " " << to_dec(b) << " " << to_hex(b) << " " << to_oct(b) << std::endl;
  std::cout << to_bin(c) << " " << to_dec(c) << " " << to_hex(c) << " " << to_oct(c) << std::endl;
  std::cout << to_bin(d) << " " << to_dec(d) << " " << to_hex(d) << " " << to_oct(d) << std::endl;
}
