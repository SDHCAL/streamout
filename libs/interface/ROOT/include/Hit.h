/**
*  \file Hit.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include <TObject.h>
#include <cstdint>

class Hit : public TObject
{
public:
  void          clear();
  void          setDIF(const std::uint8_t&);
  void          setASIC(const std::uint8_t&);
  void          setChannel(const std::uint8_t&);
  void          setThreshold(const std::uint8_t&);
  void          setDTC(const std::uint32_t&);
  void          setGTC(const std::uint32_t&);
  void          setDIFBCID(const std::uint32_t&);
  void          setFrameBCID(const std::uint32_t&);
  void          setTimestamp(const std::uint32_t&);
  void          setAbsoluteBCID(const std::uint64_t&);
  std::uint8_t  getDIFid() const;
  std::uint8_t  getASICid() const;
  std::uint8_t  getChannel() const;
  std::uint8_t  getThreshold() const;
  std::uint32_t getDTC() const;
  std::uint32_t getGTC() const;
  std::uint32_t getDIFBCID() const;
  std::uint32_t getFrameBCID() const;
  std::uint32_t getTimestamp() const;
  std::uint64_t getAbsoluteBCID() const;

private:
  std::uint8_t  m_DIF{0};
  std::uint8_t  m_ASIC{0};
  std::uint8_t  m_Channel{0};
  std::uint8_t  m_Threshold{0};
  std::uint32_t m_DTC{0};
  std::uint32_t m_GTC{0};
  std::uint32_t m_DIFBCID{0};
  std::uint32_t m_FrameBCID{0};
  std::uint32_t m_Timestamp{0};
  std::uint64_t m_AbsoluteBCID{0};
  ClassDef(Hit, 1);
};
