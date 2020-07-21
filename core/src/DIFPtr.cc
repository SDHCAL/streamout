#include "DIFPtr.h"
#include <iostream>

std::uint32_t DIFPtr::getAnalogPtr()
{
  std::uint32_t fshift{m_Shift};
  if (m_DIF[fshift]!=DU::START_OF_LINES) return fshift;
  fshift++;
  while (m_DIF[fshift]!=DU::END_OF_LINES)
  {
    m_Lines.push_back(&m_DIF[fshift]);
    std::uint32_t nchip{m_DIF[fshift]};
    fshift+=1+nchip*64*2;
  }
  return fshift++;
}



DIFPtr::DIFPtr(unsigned char* p,const std::uint32_t& maxSize,const std::uint32_t shift) : m_DIF(p),m_MaxSize(maxSize),m_Shift(shift)
{
  try
  {
    std::uint32_t fshift{0};
    if(DATA_FORMAT_VERSION>=13)
    {
      fshift=m_Shift+DU::LINES_SHIFT+1;
      if (hasTemperature()) fshift=m_Shift+DU::TDIF_SHIFT+1; // jenlev 1
      if (hasAnalogReadout()) fshift=getAnalogPtr(); // to be implemented
    }
    else std::uint32_t fshift=m_Shift+DU::BCID_SHIFT+3;
    if (m_DIF[fshift]!=DU::START_OF_FRAME)
    {
      std::cout<<"This is not a start of frame "<<m_DIF[fshift]<<"\n";
      m_FramePtr= fshift;
      return;
    }
    do
    {
      if (m_DIF[fshift]==DU::END_OF_DIF)
      {
        m_FramePtr= fshift;
        return;
      }
      if (m_DIF[fshift]==DU::START_OF_FRAME) fshift++;
      if (m_DIF[fshift]==DU::END_OF_FRAME) {fshift++;continue;}
      std::uint32_t header =getFrameAsicHeader(fshift);
      if (header == DU::END_OF_FRAME)
      {
        m_FramePtr= (fshift+2);
        return;
      }
      //std::cout<<header<<" "<<fshift<<std::endl;
      if (header<1 || header>48)
      {
        throw  header+" Header problem "+fshift;
      }
      m_Frames.push_back(&m_DIF[fshift]);fshift+=DU::FRAME_SIZE;
      if (fshift>m_MaxSize)
      {
        std::cout<<"fshift "<<fshift<<" exceed "<<m_MaxSize<<"\n";
        m_FramePtr= fshift;
        return;
      }
      if (m_DIF[fshift]==DU::END_OF_FRAME) fshift++;
    } while(true);
    
    
    
  }
  catch(const std::string& e)
  {
    std::cout<<"DIF "<<getID()<<" T ? "<<hasTemperature()<<" "<<e<<std::endl;
  }
}

void DIFPtr::dumpDIFInfo()
{
  std::cout<<"DIF"<<getID()<<" DTC "<<getDTC()<<" GTC "<<getGTC()<<" ABCID "<<getAbsoluteBCID()<<" BCID "<<getBCID()<<" Lines "<<getLines()<<" Temperature "<<hasTemperature()<<"\n";
  if (hasTemperature()) std::cout<<"T: ASU1 "<<getTASU1()<<" "<<getTemperatureASU1()<<" ASU2 "<<getTASU2()<<"  "<<getTemperatureASU2()<<" DIF "<<getTDIF()<<"  "<<getTemperatureDIF()<<"\n";
  std::cout<<"Found "<<m_Lines.size()<<" Lines and "<<m_Frames.size()<<" Frames \n";
}

unsigned long long DIFPtr::GrayToBin(const unsigned long long& n)
{
  unsigned long long ish{1};
  unsigned long long ans{n};
  unsigned long long idiv{0};
  unsigned long long ishmax{sizeof(unsigned long long)*8};
  while(true)
  {
    idiv = ans >> ish;
    ans ^= idiv;
    if (idiv <= 1 || ish == ishmax) return ans;
    ish <<= 1;
  }
}

/*bool DIFUnpacker::getFramePAD(const unsigned char* framePtr,const std::uint32_t& ip)
 * {
 *  std::uint32_t* iframe{(std::uint32_t*) &framePtr[DU::FRAME_DATA_SHIFT]};
 *  return ((iframe[3-ip/32]>>(ip%32)) & 0x1);
 * }*/



/*void DIFUnpacker::dumpFrameOld(const unsigned char* buf)
 * {
 *  bool PAD[128];
 *  bool l0[64];
 *  bool l1[64];
 *  unsigned short un{1};
 *  for(int ip= 0; ip<128; ip++){PAD[ip]=false;} //init PADs
 *  std::uint32_t idx1{4};
 *  for(int ik=0;ik<4;ik++)
 *  {
 *    unsigned long PadEtat{swap_bytes(4,&buf[idx1])};
 *    idx1+=4;
 *    for(int e=0;e<32;e++)
 *    {
 *      PAD[((3-ik)*32)+(31-e)]=PadEtat & un; //binary operation
 *      PadEtat=PadEtat>>1;	//décalage des bit de 1
 *    }
 *  }
 *  // fill bool arrays
 *  for(int p=0; p<64;p++)
 *  {
 *    l0[p]=(bool)PAD[(2*p)]; //_Lev0 (PAD paire)
 *    l1[p]=(bool)PAD[(2*p)+1]; //_Lev1 (PAD impaires)
 * 
 *  }
 *  std::bitset<64> bs0(0);
 *  std::bitset<64> bs1(0);
 *  for(std::uint32_t ip=0;ip<64;ip++) 
 *  {
 *    bs0.set(ip,l0[ip]);
 *    bs1.set(ip,l1[ip]);
 *  }
 *  std::cout<<"\t \t"<<bs0<<std::endl;
 *  std::cout<<"\t \t"<<bs1<<std::endl;
 * }*/

/*unsigned long DIFUnpacker::swap_bytes(const unsigned int& n,const unsigned char* buf)
 * {
 *  unsigned char Swapped[4];
 *  for (unsigned int i=0;i<n;i++) Swapped[i] = buf[n-1-i];
 *  unsigned long *temp=(unsigned long*) &Swapped;
 *  return (*temp);
 * }*/


