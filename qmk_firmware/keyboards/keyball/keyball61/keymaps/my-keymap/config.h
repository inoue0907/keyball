#pragma once

#define TAP_CODE_DELAY 5

// タップ判定時間 (ms)
#define TAPPING_TERM 150
#define TAPPING_TERM_PER_KEY
// キーを押しながら別のキーを打つと即座にホールドと判定
#define PERMISSIVE_HOLD
// 記号レイヤーの親指キー LT(1,KC_LNG1) だけ、押している間に別キーを押したら即ホールドにする。
// quick-tap も無効にする（moNa2 の lt_sym と同じ。keymap.c の get_* を参照）
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define QUICK_TAP_TERM_PER_KEY

// トラックボール動作時に L_MOUSE (layer 4) を自動有効化
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 4
