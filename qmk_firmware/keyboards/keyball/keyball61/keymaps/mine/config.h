// Keyball61 config.h - Ported from moNa2 ZMK config

#pragma once

// --- Tapping settings (moNa2: balanced flavor, quick-tap=0) ---
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0           // ZMK quick-tap-ms = 0 equivalent
#define PERMISSIVE_HOLD            // ZMK balanced flavor equivalent

// --- Mouse / Pointing Device ---
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE   // AML (Auto Mouse Layer) 有効化
#define AUTO_MOUSE_DEFAULT_LAYER  7         // _MOUSE layer
#define AUTO_MOUSE_TIME           700       // タイムアウト (ms)、0で無効化

// --- Keyball CPI settings (moNa2: CPI=800, SNIPE CPI=200) ---
// Keyballのデフォルト設定で調整可能
// CPI_I100/CPI_D100 キーで実行時に変更できます


// --- Vial support (Vialを使う場合はコメントを外す) ---
// #define VIAL_KEYBOARD_UID {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
// #define VIAL_UNLOCK_COMBO_ROWS {0, 0}
// #define VIAL_UNLOCK_COMBO_COLS {0, 1}
