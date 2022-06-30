/** \file RawBufferNavigator.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"
#include "DIFPtr.h"

// class to navigate in the raw data buffer
class RawBufferNavigator
{
public:
  RawBufferNavigator()  = default;
  ~RawBufferNavigator() = default;
  explicit RawBufferNavigator(const Buffer& b, const int& start = -1);
  void           setBuffer(const Buffer& b, const int& start = -1);
  std::uint8_t   getDetectorID();
  bool           validBuffer();
  std::uint32_t  getStartOfDIF();
  unsigned char* getDIFBufferStart();
  std::uint32_t  getDIFBufferSize();
  Buffer         getDIFBuffer();
  DIFPtr&        getDIFPtr();
  std::uint32_t  getEndOfDIFData();
  std::uint32_t  getSizeAfterDIFPtr();
  std::uint32_t  getDIF_CRC();
  bool           hasSlowControlData();
  Buffer         getSCBuffer();
  bool           badSCData();
  Buffer         getEndOfAllData();
  static void    StartAt(const int& start);

private:
  std::int32_t getStartOfPayload();
  void         setSCBuffer();
  Buffer       m_Buffer;
  Buffer       m_SCbuffer;
  std::int32_t m_DIFstartIndex{0};
  DIFPtr       m_TheDIFPtr;
  bool         m_BadSCdata{false};
  static int   m_Start;
};
