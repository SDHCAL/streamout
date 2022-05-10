/** \file SDHCAL_buffer.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include <cstddef>

#include "Bits.h"

class Buffer
{
public:
  Buffer(const bit8_t b[], const std::size_t& size);
  Buffer();
  Buffer(const std::size_t& max_size);
  void allocate(const std::size_t& size);
  void setValue(const std::size_t& pos,const unsigned char& value )
  {
    m_Buffer[pos]=value;

  }
  void set(unsigned char* b) { m_Buffer = b;}
  void setBegin(const std::size_t& offset) { m_offset = offset ;}
  bit8_t* begin() const { return m_Buffer+m_offset; }
  bit8_t* end() const { return m_Buffer + m_Size; }
  bit8_t operator[](const std::size_t& pos) const { return m_Buffer[pos];}
  std::size_t       size() const { return m_Size; }
  void setSize(const std::size_t& size) { m_Size = size;}
  //void           printBuffer(uint32_t start, uint32_t stop, std::ostream& flux = std::cout);
  //void           printBuffer(uint32_t start = 0, std::ostream& flux = std::cout) { printBuffer(start, size()); }
  virtual ~Buffer();
  Buffer(Buffer&&) =default;
private:
  std::size_t m_offset{0};
  Buffer(const Buffer&) =default;
  bit8_t* m_Buffer{nullptr};
  bool m_Allocate{false};
  std::size_t m_Size{0};
};
