/** \file SDHCAL_RawBuffer_Navigator.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "DIFPtr.h"
#include "SDHCAL_buffer.h"

#include <iostream>
// class to navigate in the raw data buffer
class SDHCAL_RawBuffer_Navigator
{
public:
  explicit SDHCAL_RawBuffer_Navigator(const SDHCAL_buffer& b, const int& start = -1);
  ~SDHCAL_RawBuffer_Navigator();
  bool           validBuffer();
  std::uint32_t  getStartOfDIF();
  unsigned char* getDIFBufferStart();
  std::uint32_t  getDIFBufferSize();
  SDHCAL_buffer  getDIFBuffer();
  DIFPtr*        getDIFPtr();
  std::uint32_t  getEndOfDIFData();
  std::uint32_t  getSizeAfterDIFPtr();
  std::uint32_t  getDIF_CRC();
  bool           hasSlowControlData();
  SDHCAL_buffer  getSCBuffer();
  bool           badSCData();
  SDHCAL_buffer  getEndOfAllData();
  static void    StartAt(const int& start);

private:
  void          setSCBuffer();
  SDHCAL_buffer m_Buffer{0, 0};
  SDHCAL_buffer m_SCbuffer{0, 0};
  std::uint32_t m_DIFstartIndex{0};
  DIFPtr*       m_TheDIFPtr{nullptr};
  bool          m_BadSCdata{false};
  static int    m_Start;
};
