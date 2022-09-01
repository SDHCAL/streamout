/** \file PayloadLoader.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include<memory>

#include "Payload.h"
#include "Payload100.h"
#include "Payload150.h"

#include <cstdint>

class PayloadLoader
{
public:
  PayloadLoader() = default;
  std::unique_ptr<Payload>& getPayload(const std::int32_t& detector_id)
  {
    switch(detector_id)
    {
      case 100: payload = std::make_unique<Payload100>(); break;
      case 150: payload = std::make_unique<Payload150>(); break;
    }
    return payload;
  }

private:
  std::unique_ptr<Payload> payload{nullptr};
};
