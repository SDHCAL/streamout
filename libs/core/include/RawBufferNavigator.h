/** \file RawBufferNavigator.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"

/** class to navigate in the raw data buffer
 * parse the header and send the payload as Buffer
*/

class RawBufferNavigator
{
public:
  static void StartAt(const int& start);
  RawBufferNavigator();
  ~RawBufferNavigator() = default;
  void         setBuffer(const Buffer&);
  std::uint32_t getDetectorID();
  bool         findStartOfPayload();
  std::int32_t getStartOfPayload();
  bool         validPayload();
  Buffer       getPayload();

private:
  static int   m_Start;
  Buffer       m_Buffer;
  bool         m_StartPayloadDone{false};
  std::int32_t m_StartPayload{-1};  // -1 Means not found !
};
