#pragma once

/* -- I2C - OLED --------------------------------------------- */
#define I2C_DRIVER   I2CD1
#define I2C1_SDA_PIN GP6
#define I2C1_SCL_PIN GP7

/* -- OLED --------------------------------------------------- */
#define OLED_DISPLAY_128X64
#define OLED_BRIGHTNESS      128
#define OLED_UPDATE_INTERVAL 33      /* ~30fps */

/* -- Encoder button ----------------------------------------- */
#define ENCODER_SWITCH_PIN GP26

/* Limit brightness to stay within 500mA USB budget */
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
#define RGB_MATRIX_DEFAULT_MODE       RGB_MATRIX_BREATHING
#define RGB_MATRIX_DEFAULT_HUE        170
#define RGB_MATRIX_DEFAULT_SAT        255
#define RGB_MATRIX_DEFAULT_VAL        120
#define RGB_MATRIX_DEFAULT_SPD        50

/* -- VIA ---------------------------------------------------- */
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

/* -- OLED scrolling text ------------------------------------ */
#define OLED_SCROLL_TEXT  "Still got time. Use it wisely."
#define OLED_SCROLL_SPEED 6   /* frames between each one-character shift */
