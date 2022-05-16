/** \file SDHCAL_buffer.h
 *  \copyright 2022 G.Grenier F.Lagarde A.Pingault L.Mirabito
 *  \see https://github.com/apingault/Trivent4HEP
 */

#pragma once

#include <iostream>

class Buffer
{
public:
  Buffer(): m_Buffer(nullptr), m_Size(0) {}
  Buffer(unsigned char* b, const std::size_t& i) : m_Buffer(b), m_Size(i) {}
  void           set(unsigned char* b) { m_Buffer = b; }
  unsigned char* begin() { return m_Buffer; }
  unsigned char* end() { return m_Buffer + m_Size; }
  unsigned char  operator[](const std::size_t& pos) { return m_Buffer[pos]; }
  std::size_t    size() { return m_Size; }
  void           setSize(const std::size_t& size) { m_Size = size; }
  void           printBuffer(uint32_t start, uint32_t stop, std::ostream& flux = std::cout);
  void           printBuffer(uint32_t start = 0, std::ostream& flux = std::cout) { printBuffer(start, size()); }
  virtual ~Buffer();

private:
  unsigned char* m_Buffer{nullptr};
  std::size_t    m_Size{0};
};
