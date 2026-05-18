/*
 * mona2 keymap config for keyball61
 * Based on moNa2 ZMK keymap specification
 */

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#endif

#define TAP_CODE_DELAY 5

// Match mona2's &mt tapping-term-ms = 150
#define TAPPING_TERM 150
// Match mona2's "balanced" flavor
#define PERMISSIVE_HOLD

// Auto-mouse activates layer 5 (MOUSE) on trackball movement
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 5

// Combos: ESC (pos39+38) and TAB (pos11+12) from mona2
#define COMBO_COUNT 2
#define COMBO_TERM 50
