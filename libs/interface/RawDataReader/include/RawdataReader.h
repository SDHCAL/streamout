/** \file RawdataReader.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include "Interface.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class Buffer;

class RawdataReader : public Interface
{
public:
  explicit RawdataReader(const char* fileName);
  void          start();
  void          end();
  float         getFileSize();
  void          openFile(const std::string& fileName);
  void          closeFile();
  bool          nextEvent();
  bool          nextDIFbuffer();
  const Buffer& getSDHCALBuffer();
  virtual ~RawdataReader() { closeFile(); }
  static void setDefaultBufferSize(const std::size_t& size);

private:
  void                uncompress();
  std::ifstream       m_FileStream;
  void                setFileSize(const std::size_t& size);
  static std::size_t  m_BufferSize;
  std::size_t         m_FileSize{0};
  std::uint32_t       m_NumberOfDIF{0};
  std::uint32_t       m_EventNumber{0};
  std::vector<bit8_t> m_buf;
  Buffer              m_Buffer;
  std::string         m_Filename;
};
