/*
 * mona2 keymap config for keyball61
 * Based on moNa2 ZMK keymap specification
 */

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#define TAP_CODE_DELAY 5

// Match mona2's &mt tapping-term-ms = 150
#define TAPPING_TERM 150
// Match mona2's "balanced" flavor
#define PERMISSIVE_HOLD

// Auto-mouse activates layer 5 (MOUSE) on trackball movement
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 5

// VIA: must match keymapLayerCount in mona2_via.json
// ATmega32U4 EEPROM (1KB): OLED_ENABLE increases EECONFIG_SIZE, so max 4 layers
// (4*10*8*2=640 bytes + overhead). Layers 4-7 remain in flash and work normally.
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

// Combos: ESC (pos39+38) and TAB (pos11+12) from mona2
#define COMBO_COUNT 2
#define COMBO_TERM 50
