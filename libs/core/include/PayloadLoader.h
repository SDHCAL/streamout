/** \file PayloadLoader.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include "Payload.h"
#include "Payload100.h"

#include <cstdint>

class PayloadLoader
{
public:
  PayloadLoader() = default;
  Payload* getPayload(const std::int32_t& detector_id)
  {
    switch(detector_id)
    {
      case 100: payload = new Payload100();
    }
    return payload;
  }

private:
  Payload* payload{nullptr};
};
