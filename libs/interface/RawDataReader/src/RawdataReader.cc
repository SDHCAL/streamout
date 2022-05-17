/** \file RawdataReader.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/
#include "RawdataReader.h"

#include <cstring>
#include <stdexcept>
#include <zlib.h>

/** Default size for the main buffer (taken from L.Mirabito code levbdim) **/
std::size_t RawdataReader::m_BufferSize = 0x100000;

void RawdataReader::setDefaultBufferSize(const std::size_t& size) { m_BufferSize = size; }

RawdataReader::RawdataReader(const char* fileName)
{
  m_buf.reserve(m_BufferSize);
  openFile(fileName);
}

void RawdataReader::uncompress()
{
  // static const std::size_t size_buffer{0x20000};
  /*std::size_t shift{3*sizeof(uint32_t)+sizeof(uint64_t)};
  static bit8_t obuf[size_buffer];
  std::size_t size_buffer_end;
  int rc=::uncompress(obuf,&size_buffer_end, &m_Buffer.at<bit8_t>(shift),m_Buffer.size<bit8_t>()-shift);
  memcpy(&m_Buffer.at<bit8_t>(shift),obuf,size_buffer_end);
  m_Buffer.setSize(size_buffer_end+shift);*/
}

void RawdataReader::closeFile()
{
  try
  {
    if(m_FileStream.is_open()) m_FileStream.close();
  }
  catch(const std::ios_base::failure& e)
  {
    std::cout<<"Caught an ios_base::failure in closeFile : "<<e.what()<<" "<<e.code()<<std::endl;
    throw;
  }
}

void RawdataReader::openFile(const char* fileName)
{
  try
  {
    m_FileStream.rdbuf()->pubsetbuf(0, 0);
    m_FileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_FileStream.open(fileName, std::ios::in | std::ios::binary | std::ios::ate);  // Start at the end to directly calculate the size of the file then come back to beginning
    m_FileStream.rdbuf()->pubsetbuf(0, 0);
    if(m_FileStream.is_open())
    {
      setFileSize(m_FileStream.tellg());
      m_FileStream.seekg(0, std::ios::beg);
    }
  }
  catch(const std::ios_base::failure& e)
  {
    std::cout<<"Caught an ios_base::failure in openFile : "<<e.what()<<" "<<e.code()<<std::endl;
		throw;
  }
}

bool RawdataReader::nextEvent()
{
  try
  {
    m_FileStream.read(reinterpret_cast<char*>(&m_EventNumber), sizeof(std::uint32_t));
    m_FileStream.read(reinterpret_cast<char*>(&m_NumberOfDIF), sizeof(std::uint32_t));
  }
  catch(const std::ios_base::failure& e)
  {
    return false;
  }
  return true;
}

bool RawdataReader::nextDIFbuffer()
{
  try
  {
    static int DIF_processed{0};
    if(DIF_processed >= m_NumberOfDIF)
    {
      DIF_processed = 0;
      return false;
    }
    else
    {
      DIF_processed++;
      std::uint32_t bsize{0};
      m_FileStream.read(reinterpret_cast<char*>(&bsize), sizeof(std::uint32_t));
      m_FileStream.read(reinterpret_cast<char*>(&m_buf[0]), bsize);
    }
  }
  catch(const std::ios_base::failure& e)
  {
    return false;
  }
  return true;
}

Buffer RawdataReader::getSDHCALBuffer() { return m_Buffer; }

void RawdataReader::setFileSize(const std::size_t& size) { m_FileSize = size; }

float RawdataReader::getFileSize() { return m_FileSize; }
