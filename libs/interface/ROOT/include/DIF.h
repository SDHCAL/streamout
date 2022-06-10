/**
*  \file DIF.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "Hit.h"

#include <TObject.h>
#include <cstdint>
#include <vector>

class DIF : public TObject
{
public:
  void          addHit(const Hit&);
  void          setID(const std::uint8_t&);
  std::uint8_t  getID() const;
  void          setDTC(const std::uint32_t&);
  std::uint32_t getDTC() const;
  void          setGTC(const std::uint32_t&);
  std::uint32_t getGTC() const;
  void          setDIFBCID(const std::uint32_t&);
  std::uint32_t getDIFBCID() const;
  void          setAbsoluteBCID(const std::uint64_t&);
  std::uint64_t getAbsoluteBCID() const;

private:
  std::uint8_t     m_ID{0};
  std::uint32_t    m_DTC{0};
  std::uint32_t    m_GTC{0};
  std::uint32_t    m_DIFBCID{0};
  std::uint64_t    m_AbsoluteBCID{0};
  std::vector<Hit> m_Hits;
  ClassDef(DIF, 1);
};
