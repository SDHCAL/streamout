#include "Buffer.h"

#include <iostream>

Buffer::Buffer(const bit8_t b[], const std::size_t& size) : m_Buffer(const_cast<bit8_t *>(&b[0])), m_Size(size)
{
  std::cout<<"Call constructor"<<std::endl;
}

Buffer::Buffer(): m_Buffer(nullptr), m_Size(0)
{
  std::cout<<"Call constructor"<<std::endl;
}

Buffer::Buffer(const std::size_t& max_size) : m_Size(max_size), m_Allocate(true)
{
  std::cout<<"Call constructor with allocations "<<max_size<<std::endl;
  m_Buffer = new unsigned char[max_size];
}

void Buffer::allocate(const std::size_t& size)
{
  if(m_Allocate == true && m_Size != size)
  {
    delete[] m_Buffer;
    m_Buffer = new unsigned char[size];
  }
  else if(m_Allocate != true)
  {
    std::cout<<"Call constructor with allocations "<<size<<std::endl;
    m_Buffer = new unsigned char[size];
  }
  m_Allocate =true;
  m_Size = size;
}

Buffer::~Buffer()
{
  if(m_Allocate== true && m_Buffer!=nullptr)
  {
    std::cout<<"Buffer destructor called with desallocations"<<std::endl;
    delete [] m_Buffer;
    m_Buffer = nullptr;
    m_Allocate = false;
  }
  else std::cout<<"Buffer destructor called"<<std::endl;
}
