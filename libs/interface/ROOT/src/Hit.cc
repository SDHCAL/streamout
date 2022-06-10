/**
*  \file Hit.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "Hit.h"

#include <cstdint>

void Hit::setDIF(const std::uint8_t& dif) { m_DIF = dif; }

void Hit::setASIC(const std::uint8_t& asic) { m_ASIC = asic; }

void Hit::setChannel(const std::uint8_t& channel) { m_Channel = channel; }

void Hit::setThreshold(const std::uint8_t& threshold) { m_Threshold = threshold; }

void Hit::setDTC(const std::uint32_t& dtc) { m_DTC = dtc; }

void Hit::setGTC(const std::uint32_t& gtc) { m_GTC = gtc; }

void Hit::setDIFBCID(const std::uint32_t& difbcid) { m_DIFBCID = difbcid; }

void Hit::setFrameBCID(const std::uint32_t& framebcid) { m_FrameBCID = framebcid; }

void Hit::setTimestamp(const std::uint32_t& timestamp) { m_Timestamp = timestamp; }

void Hit::setAbsoluteBCID(const std::uint64_t& absolutebcid) { m_AbsoluteBCID = absolutebcid; }

std::uint8_t Hit::getDIFid() { return m_DIF; }

std::uint8_t Hit::getASICid() { return m_ASIC; }

std::uint8_t Hit::getChannelId() { return m_Channel; }

std::uint8_t Hit::getThreshold() { return m_Threshold; }

std::uint32_t Hit::getDTC() { return m_DTC; }

std::uint32_t Hit::getGTC() { return m_GTC; }

std::uint32_t Hit::getDIFBCID() { return m_DIFBCID; }

std::uint32_t Hit::getFrameBCID() { return m_FrameBCID; }

std::uint32_t Hit::getTimestamp() { return m_Timestamp; }

std::uint64_t Hit::getAbsoluteBCID() { return m_AbsoluteBCID; }
