/** \file RawBufferNavigator.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"
#include "DIFPtr.h"
#include "spdlog/spdlog.h"

#include <memory>

// class to navigate in the raw data buffer
class RawBufferNavigator
{
public:
  explicit RawBufferNavigator(const std::shared_ptr<spdlog::logger>&);
  ~RawBufferNavigator() = default;
  explicit RawBufferNavigator(const Buffer& b);
  void          setBuffer(const Buffer& b);
  std::uint8_t  getDetectorID();
  bool          validBuffer();
  bit8_t*       getDIFBufferStart();
  std::uint32_t getDIFBufferSize();
  Buffer        getDIFBuffer();
  DIFPtr&       getDIFPtr();
  std::uint32_t getEndOfDIFData();
  std::uint32_t getSizeAfterDIFPtr();
  std::uint32_t getDIF_CRC();
  bool          hasSlowControlData();
  Buffer        getSCBuffer();
  bool          badSCData();
  Buffer        getEndOfAllData();
  static void   StartAt(const int& start);
  std::int32_t  getStartOfPayload();

private:
  std::shared_ptr<spdlog::logger> m_Logger{nullptr};
  void                            setSCBuffer();
  Buffer                          m_Buffer;
  Buffer                          m_SCbuffer;
  std::int32_t                    m_StartPayload{-1};
  DIFPtr                          m_TheDIFPtr;
  bool                            m_BadSCdata{false};
  static int                      m_Start;
};
