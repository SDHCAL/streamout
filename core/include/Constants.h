#pragma once

 constexpr int DU_DATA_FORMAT_VERSION{13};
 constexpr int DU_START_OF_DIF{0xB0};
 constexpr int DU_START_OF_DIF_TEMP{0xBB};
 constexpr int DU_END_OF_DIF{0xA0};
 constexpr int DU_START_OF_LINES{0xC4};
 constexpr int DU_END_OF_LINES{0xD4};

 constexpr int DU_START_OF_FRAME{0xB4};
 constexpr int DU_END_OF_FRAME{0xA3};

 constexpr int DU_ID_SHIFT{1};
 constexpr int DU_DTC_SHIFT{2};
 constexpr int DU_GTC_SHIFT{10};
 constexpr int DU_ABCID_SHIFT{14};
 constexpr int DU_BCID_SHIFT{20};
 constexpr int DU_LINES_SHIFT{23};
 constexpr int DU_TASU1_SHIFT{24};
 constexpr int DU_TASU2_SHIFT{28};
 constexpr int DU_TDIF_SHIFT{32};

 constexpr int DU_FRAME_ASIC_HEADER_SHIFT{0};
 constexpr int DU_FRAME_BCID_SHIFT{1};
 constexpr int DU_FRAME_DATA_SHIFT{4};
 constexpr int DU_FRAME_SIZE{20};
