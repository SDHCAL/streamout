#pragma once

class DU
{
public:
  static const std::uint32_t START_OF_DIF{0xB0};
  static const std::uint32_t START_OF_DIF_TEMP{0xBB};
  static const std::uint32_t END_OF_DIF{0xA0};
  static const std::uint32_t START_OF_LINES{0xC4};
  static const std::uint32_t END_OF_LINES{0xD4};

  static const std::uint32_t START_OF_FRAME{0xB4};
  static const std::uint32_t END_OF_FRAME{0xA3};

  static const std::uint32_t ID_SHIFT{1};
  static const std::uint32_t DTC_SHIFT{2};
  static const std::uint32_t GTC_SHIFT{10};
  static const std::uint32_t ABCID_SHIFT{14};
  static const std::uint32_t BCID_SHIFT{20};
  static const std::uint32_t LINES_SHIFT{23};
  static const std::uint32_t TASU1_SHIFT{24};
  static const std::uint32_t TASU2_SHIFT{28};
  static const std::uint32_t TDIF_SHIFT{32};

  static const std::uint32_t FRAME_ASIC_HEADER_SHIFT{0};
  static const std::uint32_t FRAME_BCID_SHIFT{1};
  static const std::uint32_t FRAME_DATA_SHIFT{4};
  static const std::uint32_t FRAME_SIZE{20};
};
