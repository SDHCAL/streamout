/** \file RawBufferNavigator.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include "Buffer.h"

/** class to navigate in the raw data buffer
 * parse the header and send the payload as Buffer
 * \class RawBufferNavigator
 * \brief the RawBufferNavigator navigate in the raw data buffer and parse the header and send the payload as Buffer.
 * \details The buffer structure consists of :
 * - the detector id (std::int32_t)
 * - the datasource id (std::int32_t)
 * - the event id (std::int32_t)
 * - the bunch crossing id (std::int64_t)
 * - the payload char array of (buffer size - 20)
 * Based on \author Laurent Mirabito
 * \version   1.0
 * \date      May 2016
*/

class RawBufferNavigator
{
public:
  static void startAt(const int& start);
  RawBufferNavigator();
  ~RawBufferNavigator() = default;
  void          setBuffer(const Buffer&);
  std::uint32_t getDetectorID();
  bool          findStartOfPayload();
  std::int32_t  getStartOfPayload();
  bool          validPayload();
  Buffer        getPayload();

private:
  Buffer              m_Buffer;
  static std::int32_t m_StartPayload;
};
