#pragma once

#include "SDHCAL_buffer.h"
#include "DIFPtr.h"
#include <iostream>
//class to navigate in the raw data buffer
class SDHCAL_RawBuffer_Navigator
{
public:
  SDHCAL_RawBuffer_Navigator(const SDHCAL_buffer& b,const int& start=-1);
  ~SDHCAL_RawBuffer_Navigator();
  bool validBuffer();
  std::uint32_t getStartOfDIF();
  unsigned char* getDIFBufferStart();
  std::uint32_t getDIFBufferSize();
  SDHCAL_buffer getDIFBuffer();
  DIFPtr* getDIFPtr();
  std::uint32_t getEndOfDIFData();
  std::uint32_t getSizeAfterDIFPtr();
  std::uint32_t getDIF_CRC();
  bool hasSlowControlData();
  SDHCAL_buffer getSCBuffer();
  bool badSCData();
  SDHCAL_buffer getEndOfAllData();
  void static StartAt(const int& start);
private:
  void setSCBuffer();
  SDHCAL_buffer m_Buffer;
  SDHCAL_buffer m_SCbuffer{0,0};
  std::uint32_t m_DIFstartIndex{0};
  DIFPtr* m_TheDIFPtr{nullptr};
  bool m_BadSCdata{false};
  static int m_Start;
};
