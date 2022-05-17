/** \file DIFPtr.CC
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#include "DIFPtr.h"

#include "spdlog/spdlog.h"

#include <string>

DIFPtr::DIFPtr(unsigned char* p, const std::uint32_t& max_size) : theDIF_(p), theSize_(max_size)
{
  theFrames_.clear();
  theLines_.clear();
  try
  {
    theGetFramePtrReturn_ = DIFUnpacker::getFramePtr(theFrames_, theLines_, theSize_, theDIF_);
  }
  catch(const std::string& e)
  {
    spdlog::get("streamout")->error(" DIF {} T ? {} {}", getID(), hasTemperature(), e);
  }
}
