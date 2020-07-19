#include "DIFUnpacker.h"

#include <sstream>
#include <iostream>
#include <stdio.h>
#include <bitset>
#include <cstring>


unsigned long long DIFUnpacker::GrayToBin(unsigned long long n)
{
unsigned long long ish, ans, idiv, ishmax;
ishmax = sizeof(unsigned long long)*8;
ish = 1;
ans = n;
while(true)
		{
			idiv = ans >> ish;
			ans ^= idiv;
			if (idiv <= 1 || ish == ishmax) return ans;
			ish <<= 1;
        }
}


uint32_t DIFUnpacker::getStartOfDIF(unsigned char* cbuf,uint32_t size_buf,uint32_t start)
{
  uint32_t id0=0;
  for (uint32_t i=start;i<size_buf;i++)
    {
      if (cbuf[i]!=DU_START_OF_DIF && cbuf[i]!=DU_START_OF_DIF_TEMP) continue;
      id0=i;
      //if (cbuf[id0+DU_ID_SHIFT]>0xFF) continue;
      break;
    }
  return id0;
}


uint32_t DIFUnpacker::getID(unsigned char* cb,uint32_t idx)
{
  return cb[idx+DU_ID_SHIFT];
}

uint32_t DIFUnpacker::getDTC(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_DTC_SHIFT]<<24)+(cb[idx+DU_DTC_SHIFT+1]<<16)+(cb[idx+DU_DTC_SHIFT+2]<<8)+cb[idx+DU_DTC_SHIFT+3];
}

uint32_t DIFUnpacker::getGTC(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_GTC_SHIFT]<<24)+(cb[idx+DU_GTC_SHIFT+1]<<16)+(cb[idx+DU_GTC_SHIFT+2]<<8)+cb[idx+DU_GTC_SHIFT+3];
}

unsigned long long DIFUnpacker::getAbsoluteBCID(unsigned char* cb,uint32_t idx)
{
  unsigned long long Shift=16777216ULL;//to shift the value from the 24 first bits
  unsigned long long LBC= ( (cb[idx+DU_ABCID_SHIFT]<<16) | (cb[idx+DU_ABCID_SHIFT+1]<<8) | (cb[idx+DU_ABCID_SHIFT+2]))*Shift+( (cb[idx+DU_ABCID_SHIFT+3]<<16) | (cb[idx+DU_ABCID_SHIFT+4]<<8) | (cb[idx+DU_ABCID_SHIFT+5]));
  return LBC;
}

uint32_t DIFUnpacker::getBCID(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_BCID_SHIFT]<<16)+(cb[idx+DU_BCID_SHIFT+1]<<8)+cb[idx+DU_BCID_SHIFT+2];
}
uint32_t DIFUnpacker::getLines(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_LINES_SHIFT]>>4)& 0x5;
}

bool DIFUnpacker::hasLine(uint32_t line,unsigned char* cb,uint32_t idx)
{
  return ((cb[idx+DU_LINES_SHIFT]>>line) & 0x1);
}

uint32_t DIFUnpacker::getTASU1(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_TASU1_SHIFT]<<24)+(cb[idx+DU_TASU1_SHIFT+1]<<16)+(cb[idx+DU_TASU1_SHIFT+2]<<8)+cb[idx+DU_TASU1_SHIFT+3];
}

uint32_t DIFUnpacker::getTASU2(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_TASU2_SHIFT]<<24)+(cb[idx+DU_TASU2_SHIFT+1]<<16)+(cb[idx+DU_TASU2_SHIFT+2]<<8)+cb[idx+DU_TASU2_SHIFT+3];
}

uint32_t DIFUnpacker::getTDIF(unsigned char* cb,uint32_t idx)
{
  return (cb[idx+DU_TDIF_SHIFT]);
}

bool DIFUnpacker::hasTemperature(unsigned char* cb,uint32_t idx)
{
  return (cb[idx]==DU_START_OF_DIF_TEMP);
}

bool DIFUnpacker::hasAnalogReadout(unsigned char* cb,uint32_t idx)
{
  return (DIFUnpacker::getLines(cb,idx)!=0);
}





uint32_t DIFUnpacker::getFrameAsicHeader(unsigned char* framePtr)
{
  return (framePtr[DU_FRAME_ASIC_HEADER_SHIFT]);
}

uint32_t DIFUnpacker::getFrameBCID(unsigned char* framePtr)
{

  unsigned long long igray=(framePtr[DU_FRAME_BCID_SHIFT]<<16)+(framePtr[DU_FRAME_BCID_SHIFT+1]<<8)+framePtr[DU_FRAME_BCID_SHIFT+2];
  return DIFUnpacker::GrayToBin(igray);
}

bool DIFUnpacker::getFramePAD(unsigned char* framePtr,uint32_t ip)
{
  uint32_t* iframe=(uint32_t*) &framePtr[DU_FRAME_DATA_SHIFT];

  return ((iframe[3-ip/32]>>(ip%32)) & 0x1);
}

bool DIFUnpacker::getFrameLevel(unsigned char* framePtr,uint32_t ip,uint32_t level)
{
  return ((framePtr[DU_FRAME_DATA_SHIFT+((3-ip/16)*4+(ip%16)/4)]>>(7-(((ip%16)%4)*2+level))) & 0x1);

  //uint32_t ibyte=((3-ip/16)*4+(ip%16)/4);
  //uint32_t ibshift=(7-(((ip%16)%4)*2+level));
  //printf("channel %d level %d byte %d bit %d \n",ip,level,ibyte,ibshift);
  //return ((framePtr[DU_FRAME_DATA_SHIFT+ibyte]>>ibshift) & 0x1);
  //return DIFUnpacker::getFramePAD(framePtr,2*ip+level);
}

uint32_t DIFUnpacker::getAnalogPtr(std::vector<unsigned char*> &vLines,unsigned char* cb,uint32_t idx)
{
  uint32_t fshift=idx;
  if (cb[fshift]!=DU_START_OF_LINES)
    return fshift;
  fshift++;
  while (cb[fshift]!=DU_END_OF_LINES)
    {
      vLines.push_back(&cb[fshift]);
      uint32_t nchip=cb[fshift];
      fshift+=1+nchip*64*2;
    }
  return fshift++;
}

uint32_t DIFUnpacker::getFramePtr(std::vector<unsigned char*> &vFrame,std::vector<unsigned char*> &vLines,uint32_t max_size,unsigned char* cb,uint32_t idx)
{
#if DU_DATA_FORMAT_VERSION>=13

  uint32_t fshift=idx+DU_LINES_SHIFT+1;
  if (DIFUnpacker::hasTemperature(cb,idx)) fshift=idx+DU_TDIF_SHIFT+1; // jenlev 1
  if (DIFUnpacker::hasAnalogReadout(cb,idx)) fshift=DIFUnpacker::getAnalogPtr(vLines,cb,fshift); // to be implemented
#else
  uint32_t fshift=idx+DU_BCID_SHIFT+3;
#endif

  // if (max_size<40)
//      {
//  for (uint32_t i=fshift;i<max_size;i++)
//    printf("%02x",cb[i]);
//  printf("\n");
  //}
  if (cb[fshift]!=DU_START_OF_FRAME)
    {
      printf("This is not a start of frame %02x \n",cb[fshift]);
      return fshift;
      //throw "Not a start of Frame";
    }



  do
    {
      // printf("fshift %d and %d \n",fshift,max_size);
      if (cb[fshift]==DU_END_OF_DIF) return fshift;
      if (cb[fshift]==DU_START_OF_FRAME) fshift++;
      if (cb[fshift]==DU_END_OF_FRAME) {fshift++;continue;}
      uint32_t header =DIFUnpacker::getFrameAsicHeader(&cb[fshift]);
      if (header == DU_END_OF_FRAME) return (fshift+2);
      //std::cout<<header<<" "<<fshift<<std::endl;
      if (header<1 || header>48)
	{
	  std::stringstream s("");
	  s<<header<<" Header problem "<<fshift<<std::endl;
	  throw  s.str();
	  return fshift;

	}
      vFrame.push_back(&cb[fshift]);fshift+=DU_FRAME_SIZE;
      if (fshift>max_size)
	{
	  printf("fshift %d exceed %d \n",fshift,max_size);
	  return fshift;
	}
      //printf("%x \n",cb[fshift]);
      if (cb[fshift]==DU_END_OF_FRAME) fshift++;

    } while (1);
}

void DIFUnpacker::dumpFrameOld(unsigned char* buf)
{
  bool PAD[128];
  bool l0[64];
  bool l1[64];
  unsigned short un = 1;

  for(int ip= 0; ip<128; ip++){PAD[ip]=0;} //init PADs
  uint32_t idx1=4;
  for(int ik=0;ik<4;ik++)
    {

      unsigned long PadEtat= swap_bytes(4,&buf[idx1]);
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
  for (uint32_t ip=0;ip<64;ip++) {bs0.set(ip,l0[ip]);bs1.set(ip,l1[ip]);}

  std::cout<<"\t \t"<<bs0<<std::endl;
  std::cout<<"\t \t"<<bs1<<std::endl;
}

unsigned long DIFUnpacker::swap_bytes(unsigned int n,unsigned char* buf)
{
  unsigned char Swapped[4];
  memset(Swapped,0,4);
  for (unsigned int i=0;i<n;i++)
    Swapped[i] = buf[n-1-i];


  unsigned long *temp =(unsigned long*) &Swapped;

  return (*temp);
}
