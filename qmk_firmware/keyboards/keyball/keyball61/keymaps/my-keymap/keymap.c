#include QMK_KEYBOARD_H
#include "quantum.h"

enum my_layers {
    L_BASE = 0, // default typing layout
    L_SYM,      // 1: symbols / F-keys
    L_NUM,      // 2: numpad / mouse navigation
    L_SET,      // 3: scroll + RGB + settings
    L_MOUSE,    // 4: auto-mouse (trackball movement activates this)
};

// 長押しで選択範囲を記号で挟むキー（moNa2 の wrap_hold と同じ動き）
//   タップ: 開き記号を 1 つ入力
//   長押し: Ctrl+X → 開き記号 → Ctrl+V → 閉じ記号（IME ON なら記号ごとに Enter で確定）
// 注意: クリップボードは選択文字列で上書きされる。VS Code などは未選択だと行全体を切り取る。
enum custom_keycodes {
    WR_DQUO = SAFE_RANGE, // "  "
    WR_QUOT,              // '  '
    WR_GRV,               // `  `
    WR_PRN,               // (  )
    WR_CBR,               // {  }
    WR_BRC,               // [  ]
};

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
    KC_ESC      , KC_F15      , KC_F16      , KC_F17      , KC_F18      , KC_F19      , KC_F20      , KC_F21      , KC_F22      , KC_F23      , KC_F24      , S(KC_MINS)  ,
    KC_DEL      , KC_Q        , KC_L        , KC_U        , KC_COMM     , KC_DOT      , KC_F        , KC_W        , KC_R        , KC_Y        , KC_P        , KC_INT3     ,
    KC_TAB      , KC_E        , KC_I        , KC_A        , KC_O        , KC_MINS     , KC_K        , KC_T        , KC_N        , KC_S        , LT(3,KC_H)  , MO(3)       ,
    KC_LSFT     , KC_Z        , KC_X        , KC_C        , KC_V        , KC_SCLN     , S(KC_SCLN)  , KC_BSPC     , KC_G        , KC_D        , KC_M        , KC_J        , KC_B        , KC_RSFT     ,
    KC_LCTL     , KC_LCTL     , KC_LGUI     , KC_LALT     , LT(1,KC_LNG1), LT(2,KC_SPC), LT(3,KC_LNG2), LSFT_T(KC_TAB), LT(2,KC_ENT), LT(1,KC_LNG2), KC_RGUI  , _______     , KC_SLSH     , KC_PSCR
  ),

  // -----------------------------------------------------------------------
  // Layer 1: Sym — 記号 / F キー
  // -----------------------------------------------------------------------
  [L_SYM] = LAYOUT_universal(
    S(KC_ESC)   , KC_F1       , KC_F2       , KC_F3       , KC_F4       , KC_F5       , S(KC_4)     , S(KC_6)     , S(KC_QUOT)  , S(KC_8)     , S(KC_9)     , S(KC_INT1)  ,
    S(KC_DEL)   , KC_F6       , KC_F7       , KC_F8       , KC_F9       , KC_F10      , S(KC_3)     , S(KC_1)     , WR_DQUO     , WR_PRN      , S(KC_0)     , S(KC_INT3)  ,
    S(KC_TAB)   , KC_F11      , KC_F12      , KC_F13      , KC_F14      , KC_F15      , S(KC_7)     , S(KC_SLSH)  , WR_QUOT     , WR_CBR      , S(KC_RBRC)  , S(KC_2)     ,
    _______     , KC_F16      , KC_F17      , KC_F18      , KC_F19      , KC_F20      , S(KC_RBRC)  , S(KC_NUHS)  , S(KC_5)     , KC_BSLS     , WR_GRV      , WR_BRC      , KC_RBRC     , S(KC_RSFT)  ,
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
  //   左の QK_BOOT は左上隅。左下は Ctrl+矢印 を打つため透過にしてある
  // -----------------------------------------------------------------------
  [L_SET] = LAYOUT_universal(
    QK_BOOT     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , KC_HOME     , C(KC_LEFT)  , C(KC_RGHT)  , KC_END      , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , KC_LEFT     , KC_DOWN     , KC_UP       , KC_RGHT     , A(C(KC_TAB)), _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , G(KC_L)     , KC_NO       , S(KC_LEFT)  , S(KC_DOWN)  , S(KC_UP)    , S(KC_RGHT)  , _______     , _______     ,
    _______     , _______     , _______     , _______     , _______     , _______     , _______     , KC_LSFT     , KC_LGUI     , _______     , _______     , _______     , _______     , QK_BOOT
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

// ---------------------------------------------------------------------------
// 記号で挟む (wrap) 機能
// ---------------------------------------------------------------------------
#define WRAP_TERM 250 // これ以上押し続けたら長押し (ms)

typedef struct {
    uint16_t open;
    uint16_t close;
} wrap_pair_t;

static const wrap_pair_t wrap_pairs[] = {
    [WR_DQUO - WR_DQUO] = {S(KC_QUOT), S(KC_QUOT)},
    [WR_QUOT - WR_DQUO] = {KC_QUOT,    KC_QUOT},
    [WR_GRV  - WR_DQUO] = {KC_GRV,     KC_GRV},
    [WR_PRN  - WR_DQUO] = {S(KC_9),    S(KC_0)},
    [WR_CBR  - WR_DQUO] = {S(KC_LBRC), S(KC_RBRC)},
    [WR_BRC  - WR_DQUO] = {KC_LBRC,    KC_RBRC},
};

static uint16_t wrap_pending = 0; // 押されていてタップ/長押しが未確定のキー（0 = なし）
static uint16_t wrap_timer   = 0;
static bool     ime_on       = false; // 最後にタップされたのが LNG1 なら ON、LNG2 なら OFF

static void wrap_send_tap(uint16_t kc) {
    tap_code16(wrap_pairs[kc - WR_DQUO].open);
}

static void wrap_send_wrap(uint16_t kc) {
    const wrap_pair_t *p = &wrap_pairs[kc - WR_DQUO];
    tap_code16(C(KC_X));
    wait_ms(30);
    tap_code16(p->open);
    if (ime_on) tap_code(KC_ENT);
    wait_ms(30);
    tap_code16(C(KC_V));
    wait_ms(30);
    tap_code16(p->close);
    if (ime_on) tap_code(KC_ENT);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // IME 状態の追跡（LT/MT のタップ側も見る）
    if (record->event.pressed) {
        uint16_t tap_kc = keycode;
        if (IS_QK_LAYER_TAP(keycode) || IS_QK_MOD_TAP(keycode)) {
            tap_kc = (record->tap.count > 0) ? (keycode & 0xFF) : KC_NO;
        }
        if (tap_kc == KC_LNG1) ime_on = true;
        if (tap_kc == KC_LNG2) ime_on = false;
    }

    // 未確定の wrap キーを押している間に別のキーが押されたらタップとして確定（速い打鍵対策）
    if (wrap_pending && record->event.pressed && keycode != wrap_pending) {
        wrap_send_tap(wrap_pending);
        wrap_pending = 0;
    }

    if (keycode >= WR_DQUO && keycode <= WR_BRC) {
        if (record->event.pressed) {
            wrap_pending = keycode;
            wrap_timer   = timer_read();
        } else if (wrap_pending == keycode) {
            wrap_send_tap(keycode); // 長押し前に離した = タップ
            wrap_pending = 0;
        }
        return false;
    }
    return true;
}

void matrix_scan_user(void) {
    if (wrap_pending && timer_elapsed(wrap_timer) >= WRAP_TERM) {
        uint16_t kc  = wrap_pending;
        wrap_pending = 0;
        wrap_send_wrap(kc);
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
