/** \file Formatters.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#ifndef FORMATTERS_H_
#define FORMATTERS_H_

#include <iosfwd>
#include <sstream>

#include "Bits.h"

class Buffer;

class to_range
{
public:
  to_range(const Buffer& buf, const std::size_t& be,const std::size_t en) : m_Buffer(buf), begin(be), end(en) {}
  const Buffer& m_Buffer;
  std::size_t begin{0};
  std::size_t end{0};
};

/** convert bit*_t in hexadecimal representation */
struct to_hexa
{
  static constexpr int HEX_DIGIT_BITS = 4;
  to_hexa(const bit8_t& v);
  to_hexa(const bit16_t& v);
  to_hexa(const bit32_t& v);
  to_hexa(const bit64_t& v);
  std::ostringstream s;
};

/** convert bit*_t in binary representation */
struct to_binary
{
  to_binary(const bit8_t& v);
  to_binary(const bit16_t& v);
  to_binary(const bit32_t& v);
  to_binary(const bit64_t& v);
  std::ostringstream s;
};

std::ostream &operator<<(std::ostream &os, const Buffer& c);
std::ostream &operator<<(std::ostream &os, const to_range& c);

#endif // FORMATTERS_H_
