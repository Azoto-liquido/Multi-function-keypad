#pragma once

/* ════════════════════════════════════════════════════════════
 *  PINOUT – XIAO RP2040
 *
 *  Keys (direct matrix, internal pull-up, active low):
 *    SW1  →  GP1   (pin 8  GPIO1/RX)
 *    SW2  →  GP2   (pin 9  GPIO2/SCK)
 *    SW3  →  GP4   (pin 10 GPIO4/MISO)
 *    SW4  →  GP3   (pin 11 GPIO3/MOSI)
 *
 *  Rotary encoder EC11 (SW5):
 *    A    →  GP29  (pin 4  GPIO29/ADC3)
 *    B    →  GP28  (pin 3  GPIO28/ADC2)
 *    S1   →  GP26  (pin 1  GPIO26/ADC0)  ← button, read by firmware
 *    S2   →  GP27  (pin 2  GPIO27/ADC1)  ← connected to GND on PCB
 *
 *  OLED SSD1306 128×64 (I2C1):
 *    SDA  →  GP6   (pin 5)
 *    SCL  →  GP7   (pin 6)
 *
 *  SK6812MINI RGBW LEDs (chain of 2):
 *    DIN  →  GP0   (pin 7  GPIO0/TX)
 *    GP0 → D2 DIN → D2 DOUT → D1 DIN → end of chain
 * ════════════════════════════════════════════════════════════ */

/* ── I2C – OLED ─────────────────────────────────────────────── */
#define I2C_DRIVER   I2CD1
#define I2C1_SDA_PIN GP6
#define I2C1_SCL_PIN GP7

/* ── OLED ───────────────────────────────────────────────────── */
#define OLED_DISPLAY_128X64
#define OLED_TIMEOUT         60000   /* turn off after 60s of inactivity */
#define OLED_BRIGHTNESS      128
#define OLED_UPDATE_INTERVAL 33      /* ~30fps */

/* ── Encoder button ─────────────────────────────────────────── */
#define ENCODER_SWITCH_PIN GP26

/* ── SK6812MINI RGBW ─────────────────────────────────────────── */
#define WS2812_DI_PIN        GP0
#define RGB_MATRIX_LED_COUNT 2

/* Limit brightness to stay within 500mA USB budget */
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
#define RGB_MATRIX_DEFAULT_MODE       RGB_MATRIX_BREATHING
#define RGB_MATRIX_DEFAULT_HUE        170
#define RGB_MATRIX_DEFAULT_SAT        255
#define RGB_MATRIX_DEFAULT_VAL        120
#define RGB_MATRIX_DEFAULT_SPD        50

/* ── VIA ────────────────────────────────────────────────────── */
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

/* ── OLED scrolling text ────────────────────────────────────── */
#define OLED_SCROLL_TEXT  "Multi Function Keypad  "
#define OLED_SCROLL_SPEED 4   /* frames between each one-character shift */
