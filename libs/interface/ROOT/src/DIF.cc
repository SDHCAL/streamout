/**
*  \file DIF.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "DIF.h"

#include <cstdint>

void DIF::addHit(const Hit& hit) { m_Hits.push_back(hit); }

void DIF::setID(const std::uint8_t& id) { m_ID = id; }

std::uint8_t DIF::getID() const { return m_ID; }

void DIF::setDTC(const std::uint32_t& dtc) { m_DTC = dtc; }

std::uint32_t DIF::getDTC() const { return m_DTC; }

void DIF::setGTC(const std::uint32_t& gtc) { m_GTC = gtc; }

std::uint32_t DIF::getGTC() const { return m_GTC; }

void DIF::setDIFBCID(const std::uint32_t& difbcid) { m_DIFBCID = difbcid; }

std::uint32_t DIF::getDIFBCID() const { return m_DIFBCID; }

void DIF::setAbsoluteBCID(const std::uint64_t& absolutebcid) { m_AbsoluteBCID = absolutebcid; }

std::uint64_t DIF::getAbsoluteBCID() const { return m_AbsoluteBCID; }
