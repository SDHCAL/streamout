#include "Formatters.h"

#include <iostream>
#include <iomanip>
#include <climits>
#include <bitset>

#include "Buffer.h"

to_hexa::to_hexa(const bit8_t& v)
{
  s<<std::hex<<std::internal<<"0x"<< std::setfill('0')<< std::setw(sizeof(bit8_t)* CHAR_BIT/HEX_DIGIT_BITS)<<v;
}

to_hexa::to_hexa(const bit16_t& v)
{
  s<<std::hex<<std::internal<<"0x"<< std::setfill('0')<< std::setw(sizeof(bit16_t)* CHAR_BIT/HEX_DIGIT_BITS)<<v;
}

to_hexa::to_hexa(const bit32_t& v)
{
  s<<std::hex<<std::internal<<"0x"<< std::setfill('0')<< std::setw(sizeof(bit32_t)* CHAR_BIT/HEX_DIGIT_BITS)<<v;
}

to_hexa::to_hexa(const bit64_t& v)
{
  s<<std::hex<<std::internal<<"0x"<< std::setfill('0')<< std::setw(sizeof(bit64_t)* CHAR_BIT/HEX_DIGIT_BITS)<<v;
}

to_binary::to_binary(const bit8_t& v)
{
  s<<std::internal<<"0b"<<std::bitset<sizeof(bit8_t)* CHAR_BIT>(v);
}

to_binary::to_binary(const bit16_t& v)
{
  s<<std::internal<<"0b"<< std::bitset<sizeof(bit16_t)* CHAR_BIT>(v);
}

to_binary::to_binary(const bit32_t& v)
{
  s<<std::internal<<"0b"<< std::bitset<sizeof(bit32_t)* CHAR_BIT>(v);
}

to_binary::to_binary(const bit64_t& v)
{
  s<<std::internal<<"0b"<< std::bitset<sizeof(bit64_t)* CHAR_BIT>(v);
}

std::ostream& operator<<(std::ostream& os, const to_binary& bf)
{
  return os << bf.s.str();
}

std::ostream& operator<<(std::ostream& os, const to_hexa& bf)
{
  return os << bf.s.str();
}

std::ostream &operator<<(std::ostream &os, const Buffer& c)
{
  for(unsigned int k = 0; k < c.size() ; k++) os << to_hexa(c[k]) << " - ";
  return os;
}
std::ostream &operator<<(std::ostream &os, const to_range& c)
{
  for(unsigned int k = c.begin; k < c.end ; k++) os << to_hexa(c.m_Buffer[k]) << " - ";
  return os;
}
