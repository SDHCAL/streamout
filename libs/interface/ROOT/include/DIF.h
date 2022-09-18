/**
*  \file DIF.h
*  \copyright 2022 G.Grenier F.Lagarde
*/
#pragma once

#include "Hit.h"

#include <TObject.h>
#include <cstdint>
#include <map>
#include <vector>

using Hits_const_iterator = std::vector<Hit>::const_iterator;

class DIF : public TObject
{
public:
  void                             clear();
  void                             addHit(const Hit&);
  void                             setID(const std::uint8_t&);
  std::uint8_t                     getID() const;
  void                             setDTC(const std::uint32_t&);
  std::uint32_t                    getDTC() const;
  void                             setDetectorID(const std::uint32_t&);
  std::uint8_t                     getDetectorID() const;
  void                             setGTC(const std::uint32_t&);
  std::uint32_t                    getGTC() const;
  void                             setDIFBCID(const std::uint32_t&);
  std::uint32_t                    getDIFBCID() const;
  void                             setAbsoluteBCID(const std::uint64_t&);
  std::uint64_t                    getAbsoluteBCID() const;
  std::vector<Hit>::const_iterator cbegin() const;
  std::vector<Hit>::const_iterator cend() const;

private:
  std::uint8_t     m_DetectorID{0};
  std::uint8_t     m_ID{0};
  std::uint32_t    m_DTC{0};
  std::uint32_t    m_GTC{0};
  std::uint32_t    m_DIFBCID{0};
  std::uint64_t    m_AbsoluteBCID{0};
  std::vector<Hit> m_Hits;
  ClassDef(DIF, 1);
};
