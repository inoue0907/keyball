/*
 * keyball61 mona2 keymap
 * Based on moNa2 ZMK keymap:
 *   e:\moNa2\zmk-config-moNa2-v2\config\mona2.keymap
 *
 * Key position mapping (mona2 pos → keyball61 key):
 *   Row 0 dropped (transparent) except R05 = pos15 (BSPC)
 *   Col 0 dropped (transparent) on all rows
 *   Main grid: mona2 row1-3 cols1-5 → keyball61 row1-3 cols1-5
 *   Thumbs: pos26→L36, pos27→R36, pos33-38→L41-L46
 *           pos39→R46, pos40→R45, pos41→R41
 */

#include QMK_KEYBOARD_H
#include "quantum.h"

enum keyball_layers {
    LAYER_DEFAULT = 0,
    LAYER_1,
    LAYER_2,
    LAYER_3,
    LAYER_4,
    LAYER_MOUSE,   // 5 — auto-activated by trackball movement
    LAYER_SCROLL,  // 6 — trackball acts as scroll wheel
    LAYER_7,       // QWERTY layout
};

enum custom_keycodes {
    CTRL_ALT_TAB = SAFE_RANGE,
};

// Combos matching mona2's combo definitions
// ESC: pos39(R46=MT-LSFT-TAB) + pos38(L46=LT3-LNG2)
const uint16_t PROGMEM combo_esc_keys[] = {MT(MOD_LSFT, KC_TAB), LT(3, KC_LNG2), COMBO_END};
// TAB: pos11(L22=I) + pos12(L23=A)
const uint16_t PROGMEM combo_tab_keys[] = {KC_I, KC_A, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_esc_keys, KC_ESC),
    COMBO(combo_tab_keys, KC_TAB),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // -------------------------------------------------------------------------
  // Layer 0: default  (mona2 Naginata-like layout)
  // -------------------------------------------------------------------------
  // LAYOUT_right_ball argument order per row:
  //   Row0: L00 L01 L02 L03 L04 L05 | R05 R04 R03 R02 R01 R00
  //   Row1: L10 L11 L12 L13 L14 L15 | R15 R14 R13 R12 R11 R10
  //   Row2: L20 L21 L22 L23 L24 L25 | R25 R24 R23 R22 R21 R20
  //   Row3: L30 L31 L32 L33 L34 L35 L36 R36 R35 R34 R33 R32 R31 R30
  //   Row4: L40 L41 L42 L43 L44 L45 L46 R46 R45 R41 R40
  [LAYER_DEFAULT] = LAYOUT_right_ball(
    //  L00      L01      L02      L03      L04      L05            R05(BSPC pos15) R04     R03      R02      R01      R00
        _______, _______, _______, _______, _______, _______,       KC_BSPC,        _______, _______, _______, _______, _______,
    //  L10      L11(Q)   L12(L)   L13(U)   L14(,)   L15(.)        R15(F)   R14(W)  R13(R)   R12(Y)   R11(P)   R10
        _______, KC_Q,    KC_L,    KC_U,    KC_COMM, KC_DOT,        KC_F,    KC_W,   KC_R,    KC_Y,    KC_P,    _______,
    //  L20      L21(E)   L22(I)   L23(A)   L24(O)   L25(-)        R25(K)   R24(T)  R23(N)   R22(S)   R21(H)   R20
        _______, KC_E,    KC_I,    KC_A,    KC_O,    KC_MINS,       KC_K,    KC_T,   KC_N,    KC_S,    LT(3, KC_H), _______,
    //  L30  L31(Z/MT)  L32(X)  L33(C)  L34(V)  L35(;)  L36(:)  R36(_)  R35(G)  R34(D)  R33(M)  R32(J)  R31(B/LT6)  R30
    //  Note: mona2 uses LT(4,COLON) but layer4 is BT-only (no QMK equivalent).
    //  KC_COLN sends Shift+; = ':' directly. Add LT(4,...) here if layer4 gets content.
        _______, MT(MOD_LSFT, KC_Z), KC_X, KC_C, KC_V, KC_SCLN, KC_COLN, KC_UNDS, KC_G, KC_D, KC_M, KC_J, LT(6, KC_B), _______,
    //  L40   L41(Ctl) L42(Win) L43(Alt) L44(LNG1/LT2) L45(SPC/LT1) L46(LNG2/LT3)  R46(TAB/MT-S) R45(ENT/LT1) R41(SLSH/LT6) R40
        _______, KC_LCTL, KC_LGUI, KC_LALT, LT(2, KC_LNG1), LT(1, KC_SPC), LT(3, KC_LNG2), MT(MOD_LSFT, KC_TAB), LT(1, KC_ENT), LT(6, KC_SLSH), _______
  ),

  // -------------------------------------------------------------------------
  // Layer 1: numpad / mouse buttons / media
  // -------------------------------------------------------------------------
  [LAYER_1] = LAYOUT_right_ball(
    //  Row0: R05=DEL
        _______, _______, _______, _______, _______, _______,       KC_DEL,  _______, _______, _______, _______, _______,
    //  _ 7 8 9 .    |    _ MB4 MPLY MB5 LCTL
        _______, _______, KC_7,    KC_8,    KC_9,    KC_DOT,        _______, KC_BTN4, KC_MPLY, KC_BTN5, KC_LCTL, _______,
    //  0 4 5 6 -    |    PGUP MB1 MB3 MB2 LALT
        _______, KC_0,    KC_4,    KC_5,    KC_6,    KC_MINS,       KC_PGUP, KC_BTN1, KC_BTN3, KC_BTN2, KC_LALT, _______,
    //  _ _ 1 2 3 /  (trns)  (trns)  PGDN C(S(TAB)) G(S(D)) C(TAB) LGUI  _
        _______, _______, KC_1,    KC_2,    KC_3,    KC_SLSH, _______, _______, KC_PGDN, LCTL(LSFT(KC_TAB)), LGUI(LSFT(KC_D)), LCTL(KC_TAB), KC_LGUI, _______,
    //  _ _ _ _ * + =   (trns)  LSFT  TO(7)  _
        _______, _______, _______, _______, KC_ASTR, KC_PLUS, KC_EQL, _______, KC_LSFT, TO(7), _______
  ),

  // -------------------------------------------------------------------------
  // Layer 2: F-keys / symbols
  // -------------------------------------------------------------------------
  [LAYER_2] = LAYOUT_right_ball(
    //  Row0: R05=CIRC (^)
        _______, _______, _______, _______, _______, _______,       KC_CIRC, _______, _______, _______, _______, _______,
    //  F1 F2 F3 F4 F5   |   # ! ' ( )
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,         KC_HASH, KC_EXLM, KC_QUOT, KC_LPRN, KC_RPRN, _______,
    //  F6 F7 F8 F9 F10  |   & ? " { }
        _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,        KC_AMPR, KC_QUES, KC_DQUO, KC_LCBR, KC_RCBR, _______,
    //  _ F11 F12 F13 F14 F15 (trns)  $  %  \  `  [  ]  _
        _______, KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15, _______, KC_DLR, KC_PERC, KC_BSLS, KC_GRV, KC_LBRC, KC_RBRC, _______,
    //  _ _ _ _ _ _ ~   @  _  |  _
        _______, _______, _______, _______, _______, _______, KC_TILD, KC_AT,   _______, KC_PIPE, _______
  ),

  // -------------------------------------------------------------------------
  // Layer 3: navigation / window management
  // -------------------------------------------------------------------------
  [LAYER_3] = LAYOUT_right_ball(
    //  Row0: R05=trans
        _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,
    //  PSCR _ _ _ A(F4)   |   HOME C(LEFT) C(RIGHT) END _
        _______, KC_PSCR, _______, _______, _______, LALT(KC_F4),   KC_HOME, LCTL(KC_LEFT), LCTL(KC_RGHT), KC_END, _______, _______,
    //  _ A(C(G(L))) C(G(L)) C(G(R)) A(C(G(R)))   |   LEFT DOWN UP RIGHT CTRL_ALT_TAB
        _______, LALT(LCTL(LGUI(KC_LEFT))), LCTL(LGUI(KC_LEFT)), LCTL(LGUI(KC_RGHT)), LALT(LCTL(LGUI(KC_RGHT))), _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, CTRL_ALT_TAB, _______,
    //  _ (tog_ls→trns) _ _ _ _ G(L)  (trns)  S(L) S(D) S(U) S(R) _  _
        _______, _______, _______, _______, _______, _______, LGUI(KC_L), _______, LSFT(KC_LEFT), LSFT(KC_DOWN), LSFT(KC_UP), LSFT(KC_RGHT), _______, _______,
    //  _ _ _ _ _ _  S(ENT)  LGUI  _  _
        _______, _______, _______, _______, _______, _______, _______, S(KC_ENT), KC_LGUI, _______, _______
  ),

  // -------------------------------------------------------------------------
  // Layer 4: BT settings (ZMK-only features → mostly transparent)
  //   R05 (sys_reset) → QK_BOOT, R36 (bootloader) → QK_BOOT
  // -------------------------------------------------------------------------
  [LAYER_4] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,       QK_BOOT, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

  // -------------------------------------------------------------------------
  // Layer 5: MOUSE (auto-activated by trackball movement)
  // -------------------------------------------------------------------------
  [LAYER_MOUSE] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,
    //  _ _ _ _ _   |   _ MB4 _ MB5 _
        _______, _______, _______, _______, _______, _______,       _______, KC_BTN4, _______, KC_BTN5, _______, _______,
    //  _ MB3 MB2 MB1 _   |   _ MB1 MB3 MB2 _
        _______, _______, KC_BTN3, KC_BTN2, KC_BTN1, _______,       _______, KC_BTN1, KC_BTN3, KC_BTN2, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

  // -------------------------------------------------------------------------
  // Layer 6: SCROLL (trackball acts as scroll wheel)
  // -------------------------------------------------------------------------
  [LAYER_SCROLL] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______,
    //  _ _ _ _ _   |   _ J K L _
        _______, _______, _______, _______, _______, _______,       _______, KC_J,    KC_K,    KC_L,    _______, _______,
    //  _ _ _ _ _ _ (trns) (trns) MPRV MRWD MPLY MFFD MNXT _
        _______, _______, _______, _______, _______, _______, _______, _______, KC_MPRV, KC_MRWD, KC_MPLY, KC_MFFD, KC_MNXT, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

  // -------------------------------------------------------------------------
  // Layer 7: QWERTY layout
  // -------------------------------------------------------------------------
  [LAYER_7] = LAYOUT_right_ball(
    //  Row0: R05=BSPC
        _______, _______, _______, _______, _______, _______,       KC_BSPC, _______, _______, _______, _______, _______,
    //  Q W E R T   |   Y U I O P
        _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
    //  A S D F G   |   H J K L ;(LT3)
        _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,    KC_J,    KC_K,    KC_L,    LT(3, KC_SCLN), _______,
    //  _ Z(MT) X C V B  -    _    N M , . :  _
        _______, MT(MOD_LSFT, KC_Z), KC_X, KC_C, KC_V, KC_B, KC_MINS, KC_UNDS, KC_N, KC_M, KC_COMM, KC_DOT, KC_COLN, _______,
    //  _ LCTL LGUI LALT LNG2(LT2) SPC(LT1) LNG1(LT3)  TAB(MT-S) ENT(LT1)  TO(1)  _
        _______, KC_LCTL, KC_LGUI, KC_LALT, LT(2, KC_LNG2), LT(1, KC_SPC), LT(3, KC_LNG1), MT(MOD_LSFT, KC_TAB), LT(1, KC_ENT), TO(1), _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Enable scroll mode when SCROLL layer (6) is active
    keyball_set_scroll_mode(get_highest_layer(state) == LAYER_SCROLL);
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CTRL_ALT_TAB:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LALT);
                wait_ms(30);
                tap_code(KC_TAB);
                wait_ms(30);
                unregister_code(KC_LALT);
                unregister_code(KC_LCTL);
            }
            return false;
    }
    return true;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
