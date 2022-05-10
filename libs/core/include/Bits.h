/**
*  \file Bits.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#ifndef BITS_H_
#define BITS_H_

#include <cstdint>
#include <iosfwd>

using bit8_t = std::uint8_t; /*<! type to represent 8bits words (1 byte) */

/** Stream operator to print bit8_t aka std::uint8_t and not char or unsigned char */
std::ostream& operator<<(std::ostream& os, const bit8_t& c);

#endif  // BITS_H_
