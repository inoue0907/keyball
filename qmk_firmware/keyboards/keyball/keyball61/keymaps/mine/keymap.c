// Keyball61 QMK Keymap - Ported from moNa2 ZMK config
// Layout macro: LAYOUT_right_ball (右ボール版)
// 必要に応じて LAYOUT や LAYOUT_left_ball に変更してください

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "keymap_japanese.h"

// --- Layer definitions ---
enum layers {
    _O24 = 0,   // Custom Dvorak-like layout (base)
    _QWERTY,    // Standard QWERTY (base)
    _NUM,       // Layer 2: Numbers & mouse buttons
    _SYM,       // Layer 3: F-keys & JIS symbols
    _NAV,       // Layer 4: Navigation (SNIPE)
    _ADJUST,    // Layer 5: Settings (Keyball CPI etc.)
    _SCROLL,    // Layer 6: Scroll mode (transparent, trackball→scroll)
    _MOUSE,     // Layer 7: Auto Mouse Layer target
};

// --- Custom keycodes ---
enum custom_keycodes {
    SEND_TIMES = QK_USER,  // Alt+0215 (× multiplication sign)
    OPEN_NOTEPAD,          // Win+R → notepad → Enter
    CMB_TOGGLE,            // Combo: toggle O24/QWERTY
};

// --- Combos ---
// NOTE: Key positions are for LAYOUT_right_ball (Keyball61)
// Row 0: pos  0- 5 (left),  6-11 (right)
// Row 1: pos 12-17 (left), 18-23 (right)
// Row 2: pos 24-29 (left), 30-35 (right)
// Row 3: pos 36-42 (left), 43-49 (right)
// Row 4: pos 50-55 (left), 56-60 (right)
//
// 実際のキーボードに合わせてポジションを調整してください

const uint16_t PROGMEM combo_esc[]       = {KC_Q, KC_L, COMBO_END};          // Q+L → ESC
const uint16_t PROGMEM combo_tab[]       = {KC_E, KC_I, COMBO_END};          // E+I → TAB
const uint16_t PROGMEM combo_tab2[]      = {KC_N, KC_S, COMBO_END};          // N+S → TAB
const uint16_t PROGMEM combo_stab[]      = {KC_T, KC_N, COMBO_END};          // T+N → Shift+TAB
const uint16_t PROGMEM combo_toggle[]    = {KC_Y, KC_P, COMBO_END};          // Y+P → Toggle O24/QWERTY
const uint16_t PROGMEM combo_altf4[]     = {KC_COMM, KC_LALT, COMBO_END};    // ,+ALT → Alt+F4
const uint16_t PROGMEM combo_ctrlshift[] = {KC_LCTL, LSFT_T(KC_Z), COMBO_END}; // Ctrl+Z(key) → Ctrl+Shift

enum combo_names {
    CMB_ESC,
    CMB_TAB,
    CMB_TAB2,
    CMB_STAB,
    CMB_TOGGLE_LAYOUT,
    CMB_ALTF4,
    CMB_CTRLSHIFT,
    COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

combo_t key_combos[] = {
    [CMB_ESC]            = COMBO(combo_esc, KC_ESC),
    [CMB_TAB]            = COMBO(combo_tab, KC_TAB),
    [CMB_TAB2]           = COMBO(combo_tab2, KC_TAB),
    [CMB_STAB]           = COMBO(combo_stab, S(KC_TAB)),
    [CMB_TOGGLE_LAYOUT]  = COMBO_ACTION(combo_toggle),
    [CMB_ALTF4]          = COMBO(combo_altf4, A(KC_F4)),
    [CMB_CTRLSHIFT]      = COMBO(combo_ctrlshift, S(KC_RCTL)),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) return;
    switch (combo_index) {
        case CMB_TOGGLE_LAYOUT:
            if (layer_state_is(_O24) || default_layer_state == (1UL << _O24)) {
                default_layer_set(1UL << _QWERTY);
            } else {
                default_layer_set(1UL << _O24);
            }
            break;
    }
}

// --- Keymap ---
// Keyball61 LAYOUT_right_ball:
//   Row 0: 6L + 6R = 12
//   Row 1: 6L + 6R = 12
//   Row 2: 6L + 6R = 12
//   Row 3: 7L + 7R = 14
//   Row 4: 6L + 5R = 11
//   Total: 61 keys

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // =============================================
    // Layer 0: O24 (Custom Dvorak-like base layer)
    // =============================================
    [_O24] = LAYOUT_right_ball(
        // Row 0 - Number row (moNa2にはない行、追加)
        KC_ESC,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,         KC_8,    KC_9,    KC_0,         KC_BSPC,
        // Row 1 - moNa2 Row 0
        KC_TAB,        KC_Q,    KC_L,    KC_U,    KC_COMM, KC_DOT,                       KC_F,    KC_W,         LT(_QWERTY,KC_R), KC_Y, KC_P,  JP_CIRC,
        // Row 2 - moNa2 Row 1
        KC_E,          KC_I,    KC_A,    KC_O,    KC_MINS, XXXXXXX,                      KC_BSPC, KC_K,         KC_T,    KC_N,    KC_S,         LT(_NUM,KC_H),
        // Row 3 - moNa2 Row 2
        LSFT_T(KC_Z),  KC_X,   KC_C,    KC_V,    KC_SCLN, LT(_ADJUST,KC_QUOT), XXXXXXX, JP_UNDS, KC_G,         KC_D,    KC_M,    KC_J,         LT(_SCROLL,KC_B), KC_SLSH,
        // Row 4 - Thumb cluster (moNa2 Row 3)
        XXXXXXX, XXXXXXX, KC_LCTL, KC_LGUI, KC_LALT, LT(_SYM,KC_LNG2),                 LT(_NUM,KC_SPC), LT(_NAV,KC_LNG1), LSFT_T(KC_TAB), LT(_NUM,KC_ENT), XXXXXXX
    ),

    // =============================================
    // Layer 1: QWERTY
    // =============================================
    [_QWERTY] = LAYOUT_right_ball(
        KC_ESC,        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,         KC_8,    KC_9,    KC_0,         KC_BSPC,
        KC_TAB,        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,         LT(_SCROLL,KC_I), KC_O, KC_P,  JP_CIRC,
        KC_A,          KC_S,    KC_D,    KC_F,    KC_G,    XXXXXXX,                      KC_BSPC, KC_H,         KC_J,    KC_K,    KC_L,         LT(_NUM,KC_SCLN),
        LSFT_T(KC_Z),  KC_X,   KC_C,    KC_V,    KC_B,    LT(_ADJUST,KC_MINS), XXXXXXX, JP_UNDS, KC_N,         KC_M,    KC_COMM, KC_DOT,       LT(_SCROLL,KC_QUOT), KC_SLSH,
        XXXXXXX, XXXXXXX, KC_LCTL, KC_LGUI, KC_LALT, LT(_SYM,KC_LNG2),                 LT(_NUM,KC_SPC), LT(_NAV,KC_LNG1), LSFT_T(KC_TAB), LT(_NUM,KC_ENT), XXXXXXX
    ),

    // =============================================
    // Layer 2: Numbers & Mouse Buttons
    // =============================================
    [_NUM] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, KC_7,    KC_8,    KC_9,    KC_DOT,                             _______, KC_BTN4, _______, KC_BTN5, _______, _______,
        KC_0,    KC_4,    KC_5,    KC_6,    KC_MINS, _______,                            KC_DEL,  KC_PGUP, KC_BTN1, KC_BTN3, KC_BTN2, _______,
        _______, KC_1,    KC_2,    KC_3,    KC_SLSH, _______, _______,                   _______, KC_PGDN, RCS(KC_TAB), _______, C(KC_TAB), _______, _______,
        _______, _______, _______, _______, JP_ASTR, JP_PLUS,                            JP_EQL,  KC_LGUI, _______, _______, _______
    ),

    // =============================================
    // Layer 3: F-keys & JIS Symbols
    // =============================================
    [_SYM] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                             JP_HASH, JP_EXLM, JP_DQUO, JP_LPRN, JP_RPRN, _______,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,                           JP_CIRC, JP_AMPR, JP_QUES, JP_QUOT, JP_LCBR, JP_RCBR,
        KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  _______, _______,                  JP_DLR,  JP_PERC, JP_BSLS, JP_GRV,  JP_LBRC, JP_RBRC, _______,
        _______, _______, _______, _______, _______, _______,                            JP_TILD, JP_AT,   _______, _______, JP_PIPE
    ),

    // =============================================
    // Layer 4: Navigation (SNIPE)
    // =============================================
    [_NAV] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,                            _______, _______,      _______,      _______,       _______, _______,
        _______, KC_PSCR, G(KC_7), G(KC_8), G(KC_9), A(KC_F4),                          KC_HOME, C(KC_LEFT),   C(KC_RGHT),   KC_END,        _______, _______,
        OPEN_NOTEPAD, G(KC_4), G(KC_5), G(KC_6), _______, _______,                      _______, KC_LEFT,      KC_DOWN,      KC_UP,         KC_RGHT, _______,
        _______, G(KC_1), G(KC_2), G(KC_3), _______, G(KC_L), _______,                  _______, S(KC_LEFT),   S(KC_DOWN),   S(KC_UP),      S(KC_RGHT), _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______,      _______,      KC_LGUI, _______
    ),

    // =============================================
    // Layer 5: Adjust (Keyball settings, was BT layer)
    // =============================================
    [_ADJUST] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            CPI_I100,CPI_I1K, SCRL_DVI,_______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            CPI_D100,CPI_D1K, SCRL_DVD,_______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,                   QK_BOOT, SCRL_TO, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______
    ),

    // =============================================
    // Layer 6: Scroll (transparent - trackball→scroll)
    // =============================================
    [_SCROLL] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______
    ),

    // =============================================
    // Layer 7: Mouse (AML target)
    // =============================================
    [_MOUSE] = LAYOUT_right_ball(
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            _______, _______, KC_BTN1, KC_BTN3, KC_BTN2, _______,
        _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                            KC_BTN1, KC_BTN2, _______, _______, _______
    ),
};

// --- Macros ---
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SEND_TIMES:
            // Alt code: × (multiplication sign, Alt+0215)
            if (record->event.pressed) {
                register_code(KC_LALT);
                tap_code(KC_KP_0);
                tap_code(KC_KP_2);
                tap_code(KC_KP_1);
                tap_code(KC_KP_5);
                unregister_code(KC_LALT);
            }
            return false;

        case OPEN_NOTEPAD:
            // Win+R → notepad → Enter
            if (record->event.pressed) {
                tap_code16(LGUI(KC_R));
                wait_ms(500);
                SEND_STRING("notepad");
                tap_code(KC_ENT);
            }
            return false;
    }
    return true;
}

// --- Keyball scroll layer integration ---
#ifdef POINTING_DEVICE_ENABLE
layer_state_t layer_state_set_user(layer_state_t state) {
    // Layer 6 (_SCROLL) でトラックボールをスクロールモードに切り替え
    keyball_set_scroll_mode(get_highest_layer(state) == _SCROLL);
    return state;
}
#endif
