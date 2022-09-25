/** \file VersionInfos.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include <utility>
#include <string>
#include "Version.h"

class VersionInfos
{
public:
  void setLibraryInfos(const std::string& name,const Version& version)
  {
    m_StreamoutLibraryInfos=std::pair<std::string,Version>{name,version};
  }
  void setReaderInfos(const std::string& name,const Version& version)
  {
    m_ReaderInfos=std::pair<std::string,Version>{name,version};
  }
  void setWriterInfos(const std::string& name,const Version& version)
  {
    m_WriterInfos=std::pair<std::string,Version>{name,version};
  }
  std::pair<std::string,Version> getLibraryInfos() const
  {
    return m_StreamoutLibraryInfos;
  }
  std::pair<std::string,Version> getReaderInfos() const
  {
    return m_ReaderInfos;
  }
  std::pair<std::string,Version> getWriterInfos() const
  {
    return m_WriterInfos;
  }
private:
  std::pair<std::string,Version> m_StreamoutLibraryInfos;
  std::pair<std::string,Version> m_ReaderInfos;
  std::pair<std::string,Version> m_WriterInfos;
};
