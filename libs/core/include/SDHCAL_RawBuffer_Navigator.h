/** \file SDHCAL_RawBuffer_Navigator.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"
#include "DIFPtr.h"

// class to navigate in the raw data buffer
class SDHCAL_RawBuffer_Navigator
{
public:
  explicit SDHCAL_RawBuffer_Navigator(const Buffer& b, const int& start = -1);
  ~SDHCAL_RawBuffer_Navigator();
  bool           validBuffer();
  std::uint32_t  getStartOfDIF();
  unsigned char* getDIFBufferStart();
  std::uint32_t  getDIFBufferSize();
  Buffer         getDIFBuffer();
  DIFPtr*        getDIFPtr();
  std::uint32_t  getEndOfDIFData();
  std::uint32_t  getSizeAfterDIFPtr();
  std::uint32_t  getDIF_CRC();
  bool           hasSlowControlData();
  Buffer         getSCBuffer();
  bool           badSCData();
  Buffer         getEndOfAllData();
  static void    StartAt(const int& start);

private:
  void          setSCBuffer();
  Buffer        m_Buffer;
  Buffer        m_SCbuffer;
  std::uint32_t m_DIFstartIndex{0};
  DIFPtr*       m_TheDIFPtr{nullptr};
  bool          m_BadSCdata{false};
  static int    m_Start;
};
