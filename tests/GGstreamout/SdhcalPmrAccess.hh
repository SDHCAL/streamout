#ifndef _SDHCALPMRACCESS_HH
#define _SDHCALPMRACCESS_HH
#include <bitset>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#define DU_DATA_FORMAT_VERSION 13

#define DU_START_OF_DIF 0xB0
#define DU_END_OF_DIF   0xA0

#define DU_START_OF_FRAME 0xB4
#define DU_END_OF_FRAME   0xA3

#define PMR_ID_SHIFT     1
#define PMR_NBASIC_SHIFT 2
#define PMR_FORMAT_SHIFT 3
#define PMR_GTC_SHIFT    4
#define PMR_ABCID_SHIFT  7
#define PMR_BCID_SHIFT   13
#define PMR_LTRG_SHIFT   16
#define PMR_HEADER_SHIFT 20

#define DU_ID_SHIFT    PMR_ID_SHIFT
#define DU_GTC_SHIFT   PMR_GTC_SHIFT
#define DU_ABCID_SHIFT PMR_ABCID_SHIFT
#define DU_BCID_SHIFT  PMR_BCID_SHIFT
#define DU_LTRG_SHIFT  PMR_LTRG_SHIFT

#define DU_FRAME_ASIC_HEADER_SHIFT 0
#define DU_FRAME_BCID_SHIFT        1
#define DU_FRAME_DATA_SHIFT        4
#define DU_FRAME_SIZE              20

#define MAX_NUMBER_OF_HIT 4000000
namespace sdhcal
{
struct pmrException : public std::exception
{
  std::string s;
  pmrException(std::string ss) : s(ss) {}
  ~pmrException() throw() {}  // Updated
  const char* what() const throw() { return s.c_str(); }
};

class PMRUnpacker
{
public:
  static unsigned long long GrayToBin(unsigned long long n);
  static uint32_t           getStartOfPMR(unsigned char* cbuf, uint32_t size_buf, uint32_t start = 92);
  static uint32_t           getID(unsigned char* cb, uint32_t idx = 0);
  static uint32_t           getGTC(unsigned char* cb, uint32_t idx = 0);
  static unsigned long long getAbsoluteBCID(unsigned char* cb, uint32_t idx = 0);
  static uint32_t           getBCID(unsigned char* cb, uint32_t idx = 0);
  static uint32_t           getLastTriggerBCID(unsigned char* cb, uint32_t idx = 0);
  static uint32_t           getFrameAsicHeader(unsigned char* framePtr);
  static void               setFrameAsicHeader(unsigned char* framePtr, uint8_t v);
  static uint32_t           getFrameBCID(unsigned char* framePtr);
  static uint32_t           getFrameBCIDW(unsigned char* framePtr);
  static uint8_t            getFrameData(unsigned char* framePtr, uint32_t ip);

  static bool getFramePAD(unsigned char* framePtr, uint32_t ip);
  static bool getFrameLevel(unsigned char* framePtr, uint32_t ip, uint32_t level);

  static uint32_t      getFramePtr(std::vector<unsigned char*>& vFrame, std::vector<unsigned char*>& vLines, uint32_t max_size, unsigned char* cb, uint32_t idx = 0);
  static void          dumpFrameOld(unsigned char* buf);
  static unsigned long swap_bytes(unsigned int n, unsigned char* buf);
};

class PMRPtr
{
public:
  PMRPtr(unsigned char* p, uint32_t max_size) : theSize_(max_size), thePMR_(p)
  {
    theFrames_.clear();
    theLines_.clear();
    try
    {
      sdhcal::PMRUnpacker::getFramePtr(theFrames_, theLines_, theSize_, thePMR_);
    }
    catch(std::string e)
    {
      std::cout << "PMR " << getID() << e << std::endl;
    }
  }
  inline unsigned char*               getPtr() { return thePMR_; }
  inline std::vector<unsigned char*>& getFramesVector() { return theFrames_; }
  inline uint32_t                     getID() { return sdhcal::PMRUnpacker::getID(thePMR_); }
  inline uint32_t                     getGTC() { return sdhcal::PMRUnpacker::getGTC(thePMR_); }
  inline unsigned long long           getAbsoluteBCID() { return sdhcal::PMRUnpacker::getAbsoluteBCID(thePMR_); }
  inline uint32_t                     getBCID() { return sdhcal::PMRUnpacker::getBCID(thePMR_); }
  inline uint32_t                     getNumberOfFrames() { return theFrames_.size(); }
  inline unsigned char*               getFramePtr(uint32_t i) { return theFrames_[i]; }
  inline uint32_t                     getFrameAsicHeader(uint32_t i) { return sdhcal::PMRUnpacker::getFrameAsicHeader(theFrames_[i]); }
  inline uint32_t                     getFrameBCID(uint32_t i) { return sdhcal::PMRUnpacker::getFrameBCID(theFrames_[i]); }
  inline uint32_t                     getFrameBCIDW(uint32_t i) { return sdhcal::PMRUnpacker::getFrameBCIDW(theFrames_[i]); }
  inline uint32_t                     getFrameTimeToTrigger(uint32_t i) { return getBCID() - getFrameBCID(i); }
  inline bool                         getFrameLevel(uint32_t i, uint32_t ipad, uint32_t ilevel) { return sdhcal::PMRUnpacker::getFrameLevel(theFrames_[i], ipad, ilevel); }
  inline uint32_t                     getFrameData(uint32_t i, uint32_t iword) { return sdhcal::PMRUnpacker::getFrameData(theFrames_[i], iword); }
  void                                dumpPMRInfo()
  {
    printf("PMR %d  GTC %d ABCID %lld BCID %d  \n", getID(), getGTC(), getAbsoluteBCID(), getBCID());
    printf("Found  %d Frames \n", (int)theFrames_.size());
  }

private:
  uint32_t                    theSize_;
  unsigned char*              thePMR_;
  std::vector<unsigned char*> theFrames_;
  std::vector<unsigned char*> theLines_;
};

};  // namespace sdhcal
#endif
