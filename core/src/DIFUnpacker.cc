#include "DIFUnpacker.h"

#include <iostream>
#include <bitset>






bool DIFUnpacker::getFramePAD(const unsigned char* framePtr,const std::uint32_t& ip)
{
  std::uint32_t* iframe{(std::uint32_t*) &framePtr[DU_FRAME_DATA_SHIFT]};
  return ((iframe[3-ip/32]>>(ip%32)) & 0x1);
}


std::uint32_t DIFUnpacker::getAnalogPtr(std::vector<unsigned char*> &vLines,unsigned char* cb,const std::uint32_t& idx)
{
  std::uint32_t fshift{idx};
  if (cb[fshift]!=DU_START_OF_LINES) return fshift;
  fshift++;
  while (cb[fshift]!=DU_END_OF_LINES)
  {
    vLines.push_back(&cb[fshift]);
    std::uint32_t nchip{cb[fshift]};
    fshift+=1+nchip*64*2;
  }
  return fshift++;
}



void DIFUnpacker::dumpFrameOld(const unsigned char* buf)
{
  bool PAD[128];
  bool l0[64];
  bool l1[64];
  unsigned short un{1};
  for(int ip= 0; ip<128; ip++){PAD[ip]=false;} //init PADs
  std::uint32_t idx1{4};
  for(int ik=0;ik<4;ik++)
  {
    unsigned long PadEtat{swap_bytes(4,&buf[idx1])};
    idx1+=4;
    for(int e=0;e<32;e++)
    {
      PAD[((3-ik)*32)+(31-e)]=PadEtat & un; //binary operation
      PadEtat=PadEtat>>1;	//décalage des bit de 1
    }
  }
  // fill bool arrays
  for(int p=0; p<64;p++)
  {
    l0[p]=(bool)PAD[(2*p)]; //_Lev0 (PAD paire)
    l1[p]=(bool)PAD[(2*p)+1]; //_Lev1 (PAD impaires)

  }
  std::bitset<64> bs0(0);
  std::bitset<64> bs1(0);
  for(std::uint32_t ip=0;ip<64;ip++) 
  {
    bs0.set(ip,l0[ip]);
    bs1.set(ip,l1[ip]);
  }
  std::cout<<"\t \t"<<bs0<<std::endl;
  std::cout<<"\t \t"<<bs1<<std::endl;
}

unsigned long DIFUnpacker::swap_bytes(const unsigned int& n,const unsigned char* buf)
{
  unsigned char Swapped[4];
  for (unsigned int i=0;i<n;i++) Swapped[i] = buf[n-1-i];
  unsigned long *temp=(unsigned long*) &Swapped;
  return (*temp);
}
