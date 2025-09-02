// copyright 2022 G.Grenier
#include "SdhcalPmrAccess.hh"

#include <bitset>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string.h>

using namespace sdhcal;
using namespace std;
unsigned long long PMRUnpacker::GrayToBin(unsigned long long n)
{
  unsigned long long ish, anss, idiv, ishmax;
  ishmax = sizeof(unsigned long long) * 8;
  ish    = 1;
  anss   = n;
  while(true)
  {
    idiv = anss >> ish;
    anss ^= idiv;
    if(idiv <= 1 || ish == ishmax) return anss;
    ish <<= 1;
  }
}

uint32_t PMRUnpacker::getStartOfPMR(unsigned char* cbuf, uint32_t size_buf, uint32_t start)
{
  uint32_t id0 = 0;

  // for (int i=0;i<128;i++) {printf("%02x ",cbuf[i]); if ((i+1)%32==0)   printf("\n");};    printf("\n");

  for(uint32_t i = start; i < size_buf; i++)
  {
    if(cbuf[i] != DU_START_OF_DIF) continue;
    id0 = i;
    // if (cbuf[id0+DU_ID_SHIFT]>0xFF) continue;
    break;
  }
  // printf("DIF %d Found at %d \n",cbuf[id0+1],id0);
  return id0;
}

uint32_t PMRUnpacker::getID(unsigned char* cb, uint32_t idx) { return cb[idx + DU_ID_SHIFT]; }

uint32_t PMRUnpacker::getGTC(unsigned char* cb, uint32_t idx) { return (cb[idx + DU_GTC_SHIFT] << 16) + (cb[idx + DU_GTC_SHIFT + 1] << 8) + cb[idx + DU_GTC_SHIFT + 2]; }

unsigned long long PMRUnpacker::getAbsoluteBCID(unsigned char* cb, uint32_t idx)
{
  unsigned long long Shift = 16777216ULL;  // to shift the value from the 24 first bits
  unsigned long long LBC   = ((cb[idx + DU_ABCID_SHIFT] << 8) | (cb[idx + DU_ABCID_SHIFT + 1])) * Shift + ((cb[idx + DU_ABCID_SHIFT + 2] << 24) | (cb[idx + DU_ABCID_SHIFT + 3] << 16) | (cb[idx + DU_ABCID_SHIFT + 4] << 8) | (cb[idx + DU_ABCID_SHIFT + 5]));
  return LBC;
}

uint32_t PMRUnpacker::getBCID(unsigned char* cb, uint32_t idx) { return (cb[idx + DU_BCID_SHIFT] << 16) + (cb[idx + DU_BCID_SHIFT + 1] << 8) + cb[idx + DU_BCID_SHIFT + 2]; }
uint32_t PMRUnpacker::getLastTriggerBCID(unsigned char* cb, uint32_t idx) { return (cb[idx + DU_LTRG_SHIFT + 1] << 16) + (cb[idx + DU_LTRG_SHIFT + 2] << 8) + cb[idx + DU_LTRG_SHIFT + 3]; }

uint32_t PMRUnpacker::getFrameAsicHeader(unsigned char* framePtr) { return (framePtr[DU_FRAME_ASIC_HEADER_SHIFT]); }
void     PMRUnpacker::setFrameAsicHeader(unsigned char* framePtr, uint8_t v) { framePtr[DU_FRAME_ASIC_HEADER_SHIFT] = v; }

uint32_t PMRUnpacker::getFrameBCID(unsigned char* framePtr)
{
  unsigned long long igray = (framePtr[DU_FRAME_BCID_SHIFT] << 16) + (framePtr[DU_FRAME_BCID_SHIFT + 1] << 8) + framePtr[DU_FRAME_BCID_SHIFT + 2];
  return PMRUnpacker::GrayToBin(igray);
}
uint32_t PMRUnpacker::getFrameBCIDW(unsigned char* framePtr)
{
  uint8_t u1  = framePtr[DU_FRAME_BCID_SHIFT];
  uint8_t u2  = framePtr[DU_FRAME_BCID_SHIFT + 1];
  uint8_t u3  = framePtr[DU_FRAME_BCID_SHIFT + 2];
  uint8_t ub1 = 0, ub2 = 0, ub3 = 0;
  for(int i = 0; i < 8; i++)
  {
    if(u1 & (1 << i)) ub1 |= (1 << (7 - i));
  }
  for(int i = 0; i < 8; i++)
  {
    if(u2 & (1 << i)) ub2 |= (1 << (7 - i));
  }
  for(int i = 0; i < 8; i++)
  {
    if(u3 & (1 << i)) ub3 |= (1 << (7 - i));
  }
  unsigned long long igray = (ub1 << 16) | (ub2 << 8) | ub3;
  return PMRUnpacker::GrayToBin(igray);
}

bool PMRUnpacker::getFramePAD(unsigned char* framePtr, uint32_t ip)
{
  uint32_t* iframe = (uint32_t*)&framePtr[DU_FRAME_DATA_SHIFT];

  return ((iframe[3 - ip / 32] >> (ip % 32)) & 0x1);
}

uint8_t PMRUnpacker::getFrameData(unsigned char* framePtr, uint32_t ip)
{
  uint8_t* iframe = (uint8_t*)&framePtr[DU_FRAME_DATA_SHIFT];
  iframe          = (uint8_t*)&framePtr[0];
  return (iframe[ip]);
}

bool PMRUnpacker::getFrameLevel(unsigned char* framePtr, uint32_t ip, uint32_t level)
{
  return ((framePtr[DU_FRAME_DATA_SHIFT + ((3 - ip / 16) * 4 + (ip % 16) / 4)] >> (7 - (((ip % 16) % 4) * 2 + level))) & 0x1);

  // uint32_t ibyte=((3-ip/16)*4+(ip%16)/4);
  // uint32_t ibshift=(7-(((ip%16)%4)*2+level));
  // printf("channel %d level %d byte %d bit %d \n",ip,level,ibyte,ibshift);
  // return ((framePtr[DU_FRAME_DATA_SHIFT+ibyte]>>ibshift) & 0x1);
  // return PMRUnpacker::getFramePAD(framePtr,2*ip+level);
}

uint32_t PMRUnpacker::getFramePtr(std::vector<unsigned char*>& vFrame, std::vector<unsigned char*>& vLines, uint32_t max_size, unsigned char* cb, uint32_t idx)
{
  uint32_t fshift = idx + PMR_HEADER_SHIFT;

  uint32_t lasth = 0, nf = 0;
  do
  {
    // printf("fshift %d and %d \n",fshift,max_size);

    /// Pas de b4 if (cb[fshift]==DU_START_OF_FRAME) fshift++;
    /// Pas de  A3 if (cb[fshift]==DU_END_OF_FRAME) {fshift++;continue;}
    uint32_t header = PMRUnpacker::getFrameAsicHeader(&cb[fshift]);
    /// Ilogique if (header == DU_END_OF_FRAME) return (fshift+2);

    //// INVERT HEADER NOt OK
    /**
      uint8_t headerb=0;
      for (int i=0;i<8;i++)
  {
    if (header & (1<<i))
      headerb|=(1<<(7-i));
  }
      // fprintf(stderr,"Header found %x %x Shift %d \n",header,headerb,fshift);
      setFrameAsicHeader(&cb[fshift],headerb);


      header=headerb;
      */
    if(cb[fshift] == DU_END_OF_DIF) return fshift;
    if(header != lasth)
    {
      // std::cout<<header<<std::endl;
      nf    = 0;
      lasth = header;
    }
#ifdef DUMPALL
    for(int j = 0; j < 20; j++) printf("%.2x ", cb[fshift + j]);
    printf("\n %d\n", nf);
#endif
    if(header < 1 || (header > 48))  // correction d'antoine pour la BIF
    {
      std::stringstream s("");
      s << header << " Header problem " << fshift << std::endl;
      // throw  pmrException(s.str());
      std::cout << s.str() << std::endl;
      // return fshift;
      for(int j = 0; j < 20; j++) printf("%.2x ", cb[fshift + j]);
      printf("\n %d\n", nf);

      getchar();
    }
    vFrame.push_back(&cb[fshift]);
    fshift += DU_FRAME_SIZE;
    nf++;
    if(fshift > max_size)
    {
      printf("fshift %d exceed %d \n", fshift, max_size);
      return fshift;
    }
    // printf("%x \n",cb[fshift]);
    /// Pas de A3 if (cb[fshift]==DU_END_OF_FRAME) fshift++;
  } while(1);
}
unsigned long PMRUnpacker::swap_bytes(unsigned int n, unsigned char* buf)
{
  unsigned char Swapped[4];
  memset(Swapped, 0, 4);
  for(unsigned int i = 0; i < n; i++) Swapped[i] = buf[n - 1 - i];

  unsigned long* temp = (unsigned long*)&Swapped;

  return (*temp);
}
void PMRUnpacker::dumpFrameOld(unsigned char* buf)
{
  bool           PAD[128];
  bool           l0[64];
  bool           l1[64];
  unsigned short un = 1;

  for(int ip = 0; ip < 128; ip++) { PAD[ip] = 0; }  // init PADs
  uint32_t idx1 = 4;
  for(int ik = 0; ik < 4; ik++)
  {
    unsigned long PadEtat = swap_bytes(4, &buf[idx1]);
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
    l0[p] = (bool)PAD[(2 * p)];      // _Lev0 (PAD paire)
    l1[p] = (bool)PAD[(2 * p) + 1];  // _Lev1 (PAD impaires)
  }
  std::bitset<64> bs0(0);
  std::bitset<64> bs1(0);
  for(uint32_t ip = 0; ip < 64; ip++)
  {
    bs0.set(ip, l0[ip]);
    bs1.set(ip, l1[ip]);
  }

  std::cout << "\t \t" << bs0 << std::endl;
  std::cout << "\t \t" << bs1 << std::endl;
}
