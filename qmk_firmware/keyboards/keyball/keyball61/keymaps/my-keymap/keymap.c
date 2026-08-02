#include QMK_KEYBOARD_H
#include "quantum.h"

enum my_layers {
    L_BASE = 0, // default typing layout
    L_SYM,      // 1: symbols / F-keys
    L_NUM,      // 2: numpad / mouse navigation
    L_SET,      // 3: scroll + RGB + settings
    L_MOUSE,    // 4: auto-mouse (trackball movement activates this)
};

// カスタムキーコード（keyball の safe range から確保）
enum my_keycodes {
    JU_TOGG = KEYBALL_SAFE_RANGE, // JIS配列 ⇔ US配列 をワンタッチで切り替え
    SELECT_LINE,                  // 現在行を先頭から末尾まで選択
};

enum tap_dances {
    TD_PAIR,
};

// -----------------------------------------------------------------------
// US/JIS 切り替えラッパー
//
// このキーマップの記号キーはすべて「OSをUS設定にした状態」で正しい文字が
// 出るように書かれている（USネイティブ）。
// jis_mode が真のときだけ、同じ見た目の文字がJIS設定OSでも出るように
// キーコードを翻訳して送出する。IMEキー(KC_LNG1/2=かな/英数)や
// もともとJIS用に置かれたキー(KC_INT1/INT3 等)、修飾キーには手を触れない。
// -----------------------------------------------------------------------
static bool jis_mode = false; // false: US(既定, ネイティブ) / true: JIS

// US設定で「ある文字」を出すキーコードを、JIS設定で同じ文字を出す
// キーコードへ変換する。該当が無ければ 0 を返す。
static uint16_t us_to_jis(uint16_t kc) {
    switch (kc) {
        // --- 修飾なし ---
        case KC_EQL:       return S(KC_MINS);  // =
        case KC_QUOT:      return S(KC_7);     // '
        case KC_LBRC:      return KC_RBRC;     // [
        case KC_RBRC:      return KC_BSLS;     // ]
        case KC_BSLS:      return KC_INT1;     // \  (US backslash → JISは KC_INT1)
        case KC_GRV:       return S(KC_LBRC);  // `
        // --- Shift 付き ---
        case S(KC_2):      return KC_LBRC;     // @
        case S(KC_6):      return KC_EQL;      // ^
        case S(KC_7):      return S(KC_6);     // &
        case S(KC_8):      return S(KC_QUOT);  // *
        case S(KC_9):      return S(KC_8);     // (
        case S(KC_0):      return S(KC_9);     // )
        case S(KC_MINS):   return S(KC_INT1);  // _
        case S(KC_EQL):    return S(KC_SCLN);  // +
        case S(KC_QUOT):   return S(KC_2);     // "
        case S(KC_SCLN):   return KC_QUOT;     // :
        case S(KC_LBRC):   return S(KC_RBRC);  // {
        case S(KC_RBRC):   return S(KC_BSLS);  // }
        case S(KC_GRV):    return S(KC_EQL);   // ~
        default:           return 0;
    }
}

// 現在のUS/JIS設定に合わせて、見た目が同じ記号を送出する。
static void tap_layout_keycode(uint16_t keycode) {
    uint16_t output = jis_mode ? us_to_jis(keycode) : 0;
    tap_code16(output != 0 ? output : keycode);
}

// 1回=()、2回=[]、3回={}。入力後はカーソルを括弧の内側へ戻す。
static bool pair_shift_held = false;

static void pair_dance_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1 && state->pressed) {
        register_mods(MOD_BIT(KC_LSFT));
        pair_shift_held = true;
        return;
    }

    switch (state->count) {
        case 1:
            tap_layout_keycode(S(KC_9));
            tap_layout_keycode(S(KC_0));
            break;
        case 2:
            tap_layout_keycode(KC_LBRC);
            tap_layout_keycode(KC_RBRC);
            break;
        default:
            tap_layout_keycode(S(KC_LBRC));
            tap_layout_keycode(S(KC_RBRC));
            break;
    }
    tap_code(KC_LEFT);
}

static void pair_dance_reset(tap_dance_state_t *state, void *user_data) {
    if (pair_shift_held) {
        unregister_mods(MOD_BIT(KC_LSFT));
        pair_shift_held = false;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_PAIR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, pair_dance_finished, pair_dance_reset),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 350;
        default:
            return TAPPING_TERM;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == JU_TOGG) {
        if (record->event.pressed) {
            jis_mode = !jis_mode;
        }
        return false;
    }

    if (keycode == SELECT_LINE) {
        if (record->event.pressed) {
            tap_code(KC_HOME);
            tap_code16(S(KC_END));
        }
        return false;
    }

    // JIS設定のときだけ記号キーを翻訳して送出する
    if (jis_mode) {
        uint16_t jis_kc = us_to_jis(keycode);
        if (jis_kc != 0) {
            if (record->event.pressed) {
                tap_code16(jis_kc);
            }
            return false; // 押下/離上とも既定処理へは流さない
        }
    }
    return true;
}

const uint16_t PROGMEM combo_esc[] = {LT(3,KC_LNG2), LSFT_T(KC_TAB), COMBO_END};
combo_t key_combos[] = {
    COMBO(combo_esc, KC_ESC),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // -----------------------------------------------------------------------
  // Layer 0: Base — oyayubi (親指シフト的) 配列
  // -----------------------------------------------------------------------
  [L_BASE] = LAYOUT_universal(
    KC_ESC      , KC_F15      , KC_F16      , KC_F17      , KC_F18      , KC_F19      , KC_F20      , KC_F21      , KC_F22      , KC_F23      , JU_TOGG     , S(KC_MINS)  ,
    KC_DEL      , KC_Q        , KC_L        , KC_U        , KC_COMM     , KC_DOT      , KC_F        , KC_W        , KC_R        , KC_Y        , KC_P        , KC_INT3     ,
    KC_TAB      , KC_E        , KC_I        , KC_A        , KC_O        , KC_MINS     , KC_K        , KC_T        , KC_N        , KC_S        , LT(3,KC_H)  , MO(3)       ,
    KC_LSFT     , KC_Z        , KC_X        , KC_C        , KC_V        , KC_SCLN     , S(KC_SCLN)  , KC_BSPC     , KC_G        , KC_D        , KC_M        , KC_J        , KC_B        , KC_RSFT     ,
    KC_LCTL     , KC_LCTL     , KC_LGUI     , KC_LALT     , LT(1,KC_LNG1), LT(2,KC_SPC), LT(3,KC_LNG2), LSFT_T(KC_TAB), LT(2,KC_ENT), LT(1,KC_LNG2), KC_RGUI  , _______     , KC_SLSH     , KC_PSCR
  ),

  // -----------------------------------------------------------------------
  // Layer 1: Sym — 記号 / F キー
  // -----------------------------------------------------------------------
  [L_SYM] = LAYOUT_universal(
    S(KC_ESC)   , KC_F1       , KC_F2       , KC_F3       , KC_F4       , KC_F5       , KC_EQL      , S(KC_6)     , S(KC_QUOT)  , S(KC_8)     , S(KC_9)     , S(KC_INT1)  ,
    S(KC_DEL)   , KC_F6       , KC_F7       , KC_F8       , KC_F9       , KC_F10      , S(KC_3)     , S(KC_1)     , S(KC_QUOT)  , S(KC_9)     , S(KC_0)     , S(KC_INT3)  ,
    S(KC_TAB)   , KC_F11      , KC_F12      , KC_F13      , KC_F14      , KC_F15      , S(KC_7)     , S(KC_SLSH)  , KC_QUOT     , S(KC_LBRC)  , S(KC_RBRC)  , S(KC_2)     ,
    _______     , KC_F16      , KC_F17      , KC_F18      , KC_F19      , KC_F20      , S(KC_RBRC)  , S(KC_NUHS)  , S(KC_5)     , KC_BSLS     , KC_GRV      , KC_LBRC     , KC_RBRC     , S(KC_RSFT)  ,
    _______     , S(KC_LCTL)  , S(KC_LALT)  , S(KC_LGUI)  , _______     , _______     , _______     , S(KC_GRV)   , S(KC_2)     , _______     , S(KC_RGUI)  , _______     , S(KC_RALT)  , _______
  ),

  // -----------------------------------------------------------------------
  // Layer 2: Num — テンキー / マウス / ウィンドウ操作
  // -----------------------------------------------------------------------
  [L_NUM] = LAYOUT_universal(
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     ,
    _______     , _______     , KC_7        , KC_8        , KC_9        , KC_DOT      , _______     , KC_BTN4     , _______     , KC_BTN5     , _______     , _______     ,
    _______     , KC_0        , KC_4        , KC_5        , KC_6        , KC_MINS     , KC_PGUP     , KC_BTN1     , KC_BTN3     , KC_BTN2     , A(C(KC_TAB)), _______     ,
    _______     , _______     , KC_1        , KC_2        , KC_3        , S(KC_MINS)  , S(KC_8)     , KC_DEL      , KC_PGDN     , S(C(KC_TAB)), G(KC_TAB)   , C(KC_TAB)   , _______     , _______     ,
    _______     , _______     , _______     , KC_DOT      , S(KC_8)     , S(KC_EQL)   , KC_EQL      , _______     , S(KC_ENT)   , _______     , _______     , _______     , _______     , _______
  ),

  // -----------------------------------------------------------------------
  // Layer 3: Set — スクロール / RGB / ナビゲーション / 設定
  //   トラックボールがスクロールホイールとして動作するレイヤー
  // -----------------------------------------------------------------------
  [L_SET] = LAYOUT_universal(
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , KC_HOME     , C(KC_LEFT)  , C(KC_RGHT)  , KC_END      , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , KC_LEFT     , KC_DOWN     , KC_UP       , KC_RGHT     , A(C(KC_TAB)), _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , G(KC_L)     , SELECT_LINE , S(KC_LEFT)  , S(KC_DOWN)  , S(KC_UP)    , S(KC_RGHT)  , _______     , _______     ,
    QK_BOOT     , _______     , _______     , _______     , _______     , _______     , _______     , TD(TD_PAIR) , KC_LGUI     , _______     , _______     , _______     , _______     , QK_BOOT
  ),

  // -----------------------------------------------------------------------
  // Layer 4: Mouse — 自動マウスレイヤー (トラックボール動作時に自動有効化)
  // -----------------------------------------------------------------------
  [L_MOUSE] = LAYOUT_universal(
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , KC_BTN4     , _______     , KC_BTN5     , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , KC_BTN1     , KC_BTN3     , KC_BTN2     , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______
  ),
};
// clang-format on

void keyball_on_apply_motion_to_mouse_scroll(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    // keyball61: m->x = 前後方向（縦スクロール源）、m->y = 左右方向（横スクロール源）
    // 縦軸から60°以内 → 縦、60°以上（横軸から30°以内）→ 横 の二択
    // tan(60°) = √3 ≈ 1.732 を整数比 7/4=1.75 で近似（誤差0.3°）
    int16_t ax = m->x < 0 ? -m->x : m->x;
    int16_t ay = m->y < 0 ? -m->y : m->y;
    if (ay * 4 < ax * 7) {
        m->y = 0;  // 縦ゾーン → 横成分を消す
    } else {
        m->x = 0;  // 横ゾーン → 縦成分を消す
    }

    // keyball標準の divmod 処理（商を返しつつ余りを m に残す）
    int16_t div = 1 << (keyball_get_scroll_div() - 1);
    int16_t x = m->x / div; m->x -= x * div;
    int16_t y = m->y / div; m->y -= y * div;

    // keyball61 軸マッピング（デフォルト実装と同じ）
#define CLIP8(v) ((v) < -127 ? -127 : (v) > 127 ? 127 : (int8_t)(v))
    r->h =  CLIP8(y);
    r->v = -CLIP8(x);
#undef CLIP8
    if (is_left) {
        r->h = -r->h;
        r->v = -r->v;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // L_SET でトラックボールをスクロールホイールとして動作させる
    keyball_set_scroll_mode(layer_state_cmp(state, L_SET));
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    keyball_handle_auto_mouse_layer_change(state);
#endif
    return state;
}
