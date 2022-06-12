/** \file DIFUnpacker.cc
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#include "DIFUnpacker.h"

#include "Formatters.h"
#include "Words.h"

#include <bitset>
#include <cstdint>
#include <iostream>
#include <spdlog/spdlog.h>

std::uint64_t DIFUnpacker::GrayToBin(const std::uint64_t& n)
{
  std::uint64_t ish{1};
  std::uint64_t anss{n};
  std::uint64_t idiv{0};
  std::uint64_t ishmax{sizeof(std::uint64_t) * 8};
  while(true)
  {
    idiv = anss >> ish;
    anss ^= idiv;
    if(idiv <= 1 || ish == ishmax) return anss;
    ish <<= 1;
  }
}

std::uint32_t DIFUnpacker::getStartOfDIF(const unsigned char* cbuf, const std::uint32_t& size_buf, const std::uint32_t& start)
{
  std::uint32_t id0{0};
  for(std::uint32_t i = start; i < size_buf; i++)
  {
    if(cbuf[i] != DU::START_OF_DIF && cbuf[i] != DU::START_OF_DIF_TEMP) continue;
    else
    {
      id0 = i;
      break;
    }
    // if (cbuf[id0+DU::ID_SHIFT]>0xFF) continue;
  }
  // std::cout << "*********************** " << id0 << std::endl;
  return id0;
}

std::uint32_t DIFUnpacker::getID(const unsigned char* cb, const std::uint32_t& idx) { return cb[idx + DU::ID_SHIFT]; }

std::uint32_t DIFUnpacker::getDTC(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::DTC_SHIFT] << 24) + (cb[idx + DU::DTC_SHIFT + 1] << 16) + (cb[idx + DU::DTC_SHIFT + 2] << 8) + cb[idx + DU::DTC_SHIFT + 3]; }

std::uint32_t DIFUnpacker::getGTC(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::GTC_SHIFT] << 24) + (cb[idx + DU::GTC_SHIFT + 1] << 16) + (cb[idx + DU::GTC_SHIFT + 2] << 8) + cb[idx + DU::GTC_SHIFT + 3]; }

std::uint64_t DIFUnpacker::getAbsoluteBCID(const unsigned char* cb, const std::uint32_t& idx)
{
  std::uint64_t Shift{16777216ULL};  // to shift the value from the 24 first bits
  std::uint64_t pos{idx + DU::ABCID_SHIFT};
  std::uint64_t LBC = ((cb[pos] << 16) | (cb[pos + 1] << 8) | (cb[pos + 2])) * Shift + ((cb[pos + 3] << 16) | (cb[pos + 4] << 8) | (cb[pos + 5]));
  return LBC;
}

std::uint32_t DIFUnpacker::getBCID(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::BCID_SHIFT] << 16) + (cb[idx + DU::BCID_SHIFT + 1] << 8) + cb[idx + DU::BCID_SHIFT + 2]; }
std::uint32_t DIFUnpacker::getLines(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::LINES_SHIFT] >> 4) & 0x5; }

bool DIFUnpacker::hasLine(const std::uint32_t& line, const unsigned char* cb, const std::uint32_t& idx) { return ((cb[idx + DU::LINES_SHIFT] >> line) & 0x1); }

std::uint32_t DIFUnpacker::getTASU1(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::TASU1_SHIFT] << 24) + (cb[idx + DU::TASU1_SHIFT + 1] << 16) + (cb[idx + DU::TASU1_SHIFT + 2] << 8) + cb[idx + DU::TASU1_SHIFT + 3]; }

std::uint32_t DIFUnpacker::getTASU2(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::TASU2_SHIFT] << 24) + (cb[idx + DU::TASU2_SHIFT + 1] << 16) + (cb[idx + DU::TASU2_SHIFT + 2] << 8) + cb[idx + DU::TASU2_SHIFT + 3]; }

std::uint32_t DIFUnpacker::getTDIF(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::TDIF_SHIFT]); }

bool DIFUnpacker::hasTemperature(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx] == DU::START_OF_DIF_TEMP); }

bool DIFUnpacker::hasAnalogReadout(const unsigned char* cb, const std::uint32_t& idx) { return (DIFUnpacker::getLines(cb, idx) != 0); }

std::uint32_t DIFUnpacker::getFrameAsicHeader(const unsigned char* framePtr) { return (framePtr[DU::FRAME_ASIC_HEADER_SHIFT]); }

std::uint32_t DIFUnpacker::getFrameBCID(const unsigned char* framePtr)
{
  std::uint32_t igray = (framePtr[DU::FRAME_BCID_SHIFT] << 16) + (framePtr[DU::FRAME_BCID_SHIFT + 1] << 8) + framePtr[DU::FRAME_BCID_SHIFT + 2];
  return DIFUnpacker::GrayToBin(igray);
}

bool DIFUnpacker::getFramePAD(const unsigned char* framePtr, const std::uint32_t& ip)
{
  std::uint32_t* iframe{(std::uint32_t*)&framePtr[DU::FRAME_DATA_SHIFT]};
  return ((iframe[3 - ip / 32] >> (ip % 32)) & 0x1);
}

bool DIFUnpacker::getFrameLevel(const unsigned char* framePtr, const std::uint32_t& ip, const std::uint32_t& level) { return ((framePtr[DU::FRAME_DATA_SHIFT + ((3 - ip / 16) * 4 + (ip % 16) / 4)] >> (7 - (((ip % 16) % 4) * 2 + level))) & 0x1); }

std::uint32_t DIFUnpacker::getAnalogPtr(std::vector<unsigned char*>& vLines, unsigned char* cb, const std::uint32_t& idx)
{
  std::uint32_t fshift{idx};
  if(cb[fshift] != DU::START_OF_LINES) return fshift;
  fshift++;
  while(cb[fshift] != DU::END_OF_LINES)
  {
    vLines.push_back(&cb[fshift]);
    std::uint32_t nchip{cb[fshift]};
    fshift += 1 + nchip * 64 * 2;
  }
  return fshift++;
}

std::uint32_t DIFUnpacker::getFramePtr(std::vector<unsigned char*>& vFrame, std::vector<unsigned char*>& vLines, const std::uint32_t& max_size, unsigned char* cb, const std::uint32_t& idx)
{
  std::uint32_t fshift{0};
  if(DATA_FORMAT_VERSION >= 13)
  {
    fshift = idx + DU::LINES_SHIFT + 1;
    if(DIFUnpacker::hasTemperature(cb, idx)) fshift = idx + DU::TDIF_SHIFT + 1;                         // jenlev 1
    if(DIFUnpacker::hasAnalogReadout(cb, idx)) fshift = DIFUnpacker::getAnalogPtr(vLines, cb, fshift);  // to be implemented
  }
  else
    fshift = idx + DU::BCID_SHIFT + 3;
  if(cb[fshift] != DU::START_OF_FRAME)
  {
    std::cout << "This is not a start of frame " << to_hex(cb[fshift]) << " \n";
    return fshift;
  }
  do {
    // printf("fshift %d and %d \n",fshift,max_size);
    if(cb[fshift] == DU::END_OF_DIF) return fshift;
    if(cb[fshift] == DU::START_OF_FRAME) fshift++;
    if(cb[fshift] == DU::END_OF_FRAME)
    {
      fshift++;
      continue;
    }
    std::uint32_t header = DIFUnpacker::getFrameAsicHeader(&cb[fshift]);
    if(header == DU::END_OF_FRAME) return (fshift + 2);
    // std::cout<<header<<" "<<fshift<<std::endl;
    if(header < 1 || header > 48) { throw header + " Header problem " + fshift; }
    vFrame.push_back(&cb[fshift]);
    fshift += DU::FRAME_SIZE;
    if(fshift > max_size)
    {
      std::cout << "fshift " << fshift << " exceed " << max_size << "\n";
      return fshift;
    }
    if(cb[fshift] == DU::END_OF_FRAME) fshift++;
  } while(true);
}
