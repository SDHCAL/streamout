#ifndef _DIFUnpacker_h
#define _DIFUnpacker_h
#include <string>
#include <vector>
#include <stdint.h>

#define DU_DATA_FORMAT_VERSION 13

#define DU_START_OF_DIF      0xB0
#define DU_START_OF_DIF_TEMP 0xBB
#define DU_END_OF_DIF        0xA0
#define DU_START_OF_LINES    0xC4
#define DU_END_OF_LINES      0xD4

#define DU_START_OF_FRAME    0xB4
#define DU_END_OF_FRAME      0xA3


#define DU_ID_SHIFT      1
#define DU_DTC_SHIFT     2
#define DU_GTC_SHIFT    10
#define DU_ABCID_SHIFT  14
#define DU_BCID_SHIFT   20
#define DU_LINES_SHIFT  23
#define DU_TASU1_SHIFT  24
#define DU_TASU2_SHIFT  28
#define DU_TDIF_SHIFT   32

#define DU_FRAME_ASIC_HEADER_SHIFT 0
#define DU_FRAME_BCID_SHIFT        1
#define DU_FRAME_DATA_SHIFT        4
#define DU_FRAME_SIZE              20

class DIFUnpacker {
public:
  static unsigned long long GrayToBin(unsigned long long n);
  static uint32_t getStartOfDIF(unsigned char* cbuf,uint32_t size_buf,uint32_t start=92);
  static uint32_t getID(unsigned char* cb,uint32_t idx=0);
  static uint32_t getDTC(unsigned char* cb,uint32_t idx=0);
  static uint32_t getGTC(unsigned char* cb,uint32_t idx=0);
  static unsigned long long getAbsoluteBCID(unsigned char* cb,uint32_t idx=0);
  static uint32_t getBCID(unsigned char* cb,uint32_t idx=0);
  static uint32_t getLines(unsigned char* cb,uint32_t idx=0);
  static bool hasLine(uint32_t line,unsigned char* cb,uint32_t idx=0);
  static uint32_t getTASU1(unsigned char* cb,uint32_t idx=0);
  static uint32_t getTASU2(unsigned char* cb,uint32_t idx=0);
  static uint32_t getTDIF(unsigned char* cb,uint32_t idx=0);
  static bool hasTemperature(unsigned char* cb,uint32_t idx=0);
  static bool hasAnalogReadout(unsigned char* cb,uint32_t idx=0);

  static uint32_t getFrameAsicHeader(unsigned char* framePtr);
  static uint32_t getFrameBCID(unsigned char* framePtr);

  static bool getFramePAD(unsigned char* framePtr,uint32_t ip);
  static bool getFrameLevel(unsigned char* framePtr,uint32_t ip,uint32_t level);

  static uint32_t getAnalogPtr(std::vector<unsigned char*> &vLines,unsigned char* cb,uint32_t idx=0) throw (std::string);
  static uint32_t getFramePtr(std::vector<unsigned char*> &vFrame,std::vector<unsigned char*> &vLines,uint32_t max_size,unsigned char* cb,uint32_t idx=0) throw (std::string);
  static void dumpFrameOld(unsigned char* buf);
  static unsigned long swap_bytes(unsigned int n,unsigned char* buf); //Stolen from DCBufferReader

};
#endif
