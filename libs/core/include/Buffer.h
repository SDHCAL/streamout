/** \file Buffer.h
 *  \copyright 2022 G.Grenier F.Lagarde A.Pingault L.Mirabito
 *  \see https://github.com/apingault/Trivent4HEP
 */

#pragma once

#include "Bits.h"

#include <array>
#include <vector>

class Buffer
{
public:
  Buffer() : m_Buffer(nullptr), m_Size(0), m_Capacity(0) {}
  Buffer(const bit8_t b[], const std::size_t& i) : m_Buffer(const_cast<bit8_t*>(&b[0])), m_Size(i), m_Capacity(i) {}
  Buffer(const char b[], const std::size_t& i) : m_Buffer(const_cast<bit8_t*>(reinterpret_cast<const bit8_t*>(&b[0]))), m_Size(i), m_Capacity(i) {}
  template<typename T> Buffer(const std::vector<T>& rawdata) : m_Buffer(const_cast<bit8_t*>(reinterpret_cast<const bit8_t*>(rawdata.data()))), m_Size(rawdata.size() * sizeof(T)), m_Capacity(rawdata.capacity() * sizeof(T)) {}
  template<typename T, std::size_t N> Buffer(const std::array<T, N>& rawdata) : m_Buffer(const_cast<bit8_t*>(reinterpret_cast<const bit8_t*>(rawdata.data()))), m_Size(rawdata.size() * sizeof(T)), m_Capacity(rawdata.size() * sizeof(T)) {}

  std::size_t size() const { return m_Size; }
  std::size_t capacity() const { return m_Capacity; }

  void    set(unsigned char* b) { m_Buffer = b; }
  bit8_t* begin() const { return m_Buffer; }
  bit8_t* end() const { return m_Buffer + m_Size; }
  bit8_t& operator[](const std::size_t& pos) { return m_Buffer[pos]; }
  bit8_t& operator[](const std::size_t& pos) const { return m_Buffer[pos]; }

  void setSize(const std::size_t& size) { m_Size = size; }
  virtual ~Buffer();

private:
  bit8_t*     m_Buffer{nullptr};
  std::size_t m_Size{0};
  std::size_t m_Capacity{0};
};
