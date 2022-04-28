/** \file DIFUnpacker.cc
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#include "DIFUnpacker.h"

#include "Words.h"

#include <bitset>
#include <cstdint>
#include <iostream>

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
    id0 = i;
    // if (cbuf[id0+DU::ID_SHIFT]>0xFF) continue;
    break;
  }
  return id0;
}

std::uint32_t DIFUnpacker::getID(const unsigned char* cb, const std::uint32_t& idx) { return cb[idx + DU::ID_SHIFT]; }

std::uint32_t DIFUnpacker::getDTC(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::DTC_SHIFT] << 24) + (cb[idx + DU::DTC_SHIFT + 1] << 16) + (cb[idx + DU::DTC_SHIFT + 2] << 8) + cb[idx + DU::DTC_SHIFT + 3]; }

std::uint32_t DIFUnpacker::getGTC(const unsigned char* cb, const std::uint32_t& idx) { return (cb[idx + DU::GTC_SHIFT] << 24) + (cb[idx + DU::GTC_SHIFT + 1] << 16) + (cb[idx + DU::GTC_SHIFT + 2] << 8) + cb[idx + DU::GTC_SHIFT + 3]; }

std::uint64_t DIFUnpacker::getAbsoluteBCID(const unsigned char* cb, const std::uint32_t& idx)
{
  std::uint64_t Shift{16777216ULL};  // to shift the value from the 24 first bits
  std::uint64_t LBC = ((cb[idx + DU::ABCID_SHIFT] << 16) | (cb[idx + DU::ABCID_SHIFT + 1] << 8) | (cb[idx + DU::ABCID_SHIFT + 2])) * Shift + ((cb[idx + DU::ABCID_SHIFT + 3] << 16) | (cb[idx + DU::ABCID_SHIFT + 4] << 8) | (cb[idx + DU::ABCID_SHIFT + 5]));
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
  unsigned long long igray = (framePtr[DU::FRAME_BCID_SHIFT] << 16) + (framePtr[DU::FRAME_BCID_SHIFT + 1] << 8) + framePtr[DU::FRAME_BCID_SHIFT + 2];
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
    std::uint32_t fshift = idx + DU::BCID_SHIFT + 3;
  if(cb[fshift] != DU::START_OF_FRAME)
  {
    std::cout << "This is not a start of frame " << cb[fshift] << "\n";
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

void DIFUnpacker::dumpFrameOld(const unsigned char* buf)
{
  bool           PAD[128];
  bool           l0[64];
  bool           l1[64];
  unsigned short un{1};
  for(int ip = 0; ip < 128; ip++) { PAD[ip] = false; }  // init PADs
  std::uint32_t idx1{4};
  for(int ik = 0; ik < 4; ik++)
  {
    unsigned long PadEtat{swap_bytes(4, &buf[idx1])};
    idx1 += 4;
    for(int e = 0; e < 32; e++)
    {
      PAD[((3 - ik) * 32) + (31 - e)] = PadEtat & un;  // binary operation
      PadEtat                         = PadEtat >> 1;  // décalage des bit de 1
    }
  }
  // fill bool arrays
  for(int p = 0; p < 64; p++)
  {
    l0[p] = static_cast<bool>(PAD[(2 * p)]);      // _Lev0 (PAD paire)
    l1[p] = static_cast<bool>(PAD[(2 * p) + 1]);  // _Lev1 (PAD impaires)
  }
  std::bitset<64> bs0(0);
  std::bitset<64> bs1(0);
  for(std::uint32_t ip = 0; ip < 64; ip++)
  {
    bs0.set(ip, l0[ip]);
    bs1.set(ip, l1[ip]);
  }
  std::cout << "\t \t" << bs0 << std::endl;
  std::cout << "\t \t" << bs1 << std::endl;
}

unsigned long DIFUnpacker::swap_bytes(const unsigned int& n, const unsigned char* buf)
{
  unsigned char Swapped[4];
  for(unsigned int i = 0; i < n; i++) Swapped[i] = buf[n - 1 - i];
  unsigned long* temp = (unsigned long*)&Swapped;
  return (*temp);
}
