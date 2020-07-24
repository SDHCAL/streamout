#include "DIFPtr.h"

DIFPtr::DIFPtr(unsigned char* p,const std::uint32_t& max_size) : theDIF_(p),theSize_(max_size)
{
  theFrames_.clear();theLines_.clear();
  try
  {
    theGetFramePtrReturn_=DIFUnpacker::getFramePtr(theFrames_,theLines_,theSize_,theDIF_);
  }
  catch (std::string e)
  {
    std::cout<<"DIF "<<getID()<<" T ? "<<hasTemperature()<<" " <<e<<std::endl;
  }
}
