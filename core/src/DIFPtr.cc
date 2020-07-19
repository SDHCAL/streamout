#include "DIFPtr.h"
#include <iostream>

DIFPtr::DIFPtr(unsigned char* p,const uint32_t& max_size,const std::uint32_t& idx) : theDIF_(p),theSize_(max_size)
{
  theFrames_.clear();theLines_.clear();
  try
  {
    #if DU_DATA_FORMAT_VERSION>=13
    std::uint32_t fshift{idx+DU_LINES_SHIFT+1};
    if (hasTemperature(theDIF_,idx)) fshift{idx+DU_TDIF_SHIFT+1}; // jenlev 1
    if (hasAnalogReadout(theDIF_,idx)) fshift{getAnalogPtr(theLines_,theDIF_,fshift)}; // to be implemented
    #else
    std::uint32_t fshift{idx+DU_BCID_SHIFT+3};
    #endif
    if (theDIF_[fshift]!=DU_START_OF_FRAME)
    {
      std::cout<<"This is not a start of frame "<<theDIF_[fshift]<<"\n";
      theGetFramePtrReturn_=fshift;
    }
    do
    {
      // printf("fshift %d and %d \n",fshift,max_size);
      if (theDIF_[fshift]==DU_END_OF_DIF) theGetFramePtrReturn_= fshift;
      if (theDIF_[fshift]==DU_START_OF_FRAME) fshift++;
      if (theDIF_[fshift]==DU_END_OF_FRAME) {fshift++;continue;}
      std::uint32_t header =getFrameAsicHeader(fshift);
      if (header == DU_END_OF_FRAME) theGetFramePtrReturn_= (fshift+2);
      //std::cout<<header<<" "<<fshift<<std::endl;
      if (header<1 || header>48)
      {
        throw  header+" Header problem "+fshift;
      }
      theFrames_.push_back(&theDIF_[fshift]);fshift+=DU_FRAME_SIZE;
      if (fshift>max_size)
      {
        std::cout<<"fshift "<<fshift<<" exceed "<<max_size<<"\n";
        theGetFramePtrReturn_= fshift;
      }
      if (theDIF_[fshift]==DU_END_OF_FRAME) fshift++;
    } while(true);
    
    
    

    //theGetFramePtrReturn_=DIFUnpacker::getFramePtr(theFrames_,theLines_,theSize_,theDIF_);
  }
  catch(const std::string& e)
  {
    std::cout<<"DIF "<<getID()<<" T ? "<<hasTemperature()<<" " <<e<<std::endl;
  }
}

void DIFPtr::dumpDIFInfo()
{
  printf("DIF %d DTC %d GTC %d ABCID %lld BCID %d Lines %d Temperature %d \n",
         getID(),
         getDTC(),
         getGTC(),
         getAbsoluteBCID(),
         getBCID(),
         getLines(),
         hasTemperature());
  
  if(hasTemperature())
    printf("T: ASU1 %d %f ASU2 %d %f DIF %d  %f \n",getTASU1(),getTemperatureASU1(),getTASU2(),getTemperatureASU2(),getTDIF(),getTemperatureDIF());
  printf("Found %ld Lines and %ld Frames \n",theLines_.size(),theFrames_.size());
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

unsigned long DIFPtr::swap_bytes(const unsigned int& n,const unsigned char* buf)
{
  unsigned char Swapped[4];
  for (unsigned int i=0;i<n;i++) Swapped[i] = buf[n-1-i];
  unsigned long *temp=(unsigned long*) &Swapped;
  return (*temp);
}
