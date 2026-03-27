/* ============================================================
 *  Multi Function Keypad – VIA keymap
 *
 *  Physical keys:
 *    SW1 (GP1)  SW2 (GP2)  SW3 (GP4)  SW4 (GP3)
 *
 *  Rotary encoder EC11:
 *    Turn CW   → Volume Up
 *    Turn CCW  → Volume Down
 *    Click GP26 → Mute toggle
 *
 *  Custom keycodes:
 *    CK_MUTE    – mute toggle (remappable from VIA)
 *    CK_OLED_IN – activates OLED text input mode:
 *                 type text → Enter to confirm, Esc to cancel
 * ============================================================ */

#include QMK_KEYBOARD_H
#include "multi_function_keypad.h"
#include "gpio.h"

/* ── Custom keycodes ─────────────────────────────────────── */
enum custom_keycodes {
    CK_MUTE    = QK_KB_0,
    CK_OLED_IN = QK_KB_1,
};

/* ── Layer ───────────────────────────────────────────────── */
enum layers {
    _BASE = 0,
    _FN1,
    _FN2,
    _FN3,
};

/* ── Keymap ──────────────────────────────────────────────── */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*  SW1         SW2         SW3         SW4       */
    [_BASE] = LAYOUT(
        KC_MPLY,    KC_MSTP,    KC_MPRV,    MO(_FN1)
    ),
    [_FN1] = LAYOUT(
        KC_MNXT,    CK_MUTE,    CK_OLED_IN, _______
    ),
    [_FN2] = LAYOUT(
        _______,    _______,    _______,    _______
    ),
    [_FN3] = LAYOUT(
        _______,    _______,    _______,    _______
    ),
};

/* ── Encoder map ─────────────────────────────────────────── */
#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN1]  = { ENCODER_CCW_CW(RM_VALD, RM_VALU)  },
    [_FN2]  = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT)  },
    [_FN3]  = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP)  },
};
#endif

/* ── Init ────────────────────────────────────────────────── */
void keyboard_post_init_user(void) {
    gpio_set_pin_input_high(GP26);   /* encoder button S1, internal pull-up */
}

/* ── Encoder button polling on GP26 ─────────────────────── */
static bool enc_btn_prev = true;

void matrix_scan_user(void) {
    bool enc_btn = gpio_read_pin(GP26);
    if (enc_btn_prev && !enc_btn) {
        /* Falling edge = button pressed */
        is_muted = !is_muted;
        tap_code(KC_MUTE);
    }
    enc_btn_prev = enc_btn;
}

/* ── OLED text input mode ────────────────────────────────── */
static bool    oled_input_active = false;
static char    oled_input_buf[64];
static uint8_t oled_input_len    = 0;

static char keycode_to_char(uint16_t kc) {
    if (kc >= KC_A   && kc <= KC_Z)  return 'a' + (kc - KC_A);
    if (kc >= KC_1   && kc <= KC_9)  return '1' + (kc - KC_1);
    if (kc == KC_0)                  return '0';
    if (kc == KC_SPACE)              return ' ';
    if (kc == KC_MINUS)              return '-';
    if (kc == KC_DOT)                return '.';
    if (kc == KC_SLASH)              return '/';
    if (kc == KC_COMMA)              return ',';
    if (kc == KC_EQUAL)              return '=';
    if (kc == KC_EXCLAIM)            return '!';
    return 0;
}

/* ── process_record ──────────────────────────────────────── */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (keycode == CK_MUTE && record->event.pressed) {
        is_muted = !is_muted;
        tap_code(KC_MUTE);
        return false;
    }

    if (keycode == CK_OLED_IN && record->event.pressed) {
        oled_input_active = true;
        oled_input_len    = 0;
        memset(oled_input_buf, 0, sizeof(oled_input_buf));
        return false;
    }

    if (oled_input_active && record->event.pressed) {
        switch (keycode) {
            case KC_ENT:
                oled_input_active = false;
                if (oled_input_len > 0)
                    oled_set_scroll_text(oled_input_buf);
                return false;

            case KC_ESC:
                oled_input_active = false;
                return false;

            case KC_BSPC:
                if (oled_input_len > 0)
                    oled_input_buf[--oled_input_len] = '\0';
                return false;

            default: {
                char c = keycode_to_char(keycode);
                if (c >= 'a' && c <= 'z' && (get_mods() & MOD_MASK_SHIFT))
                    c -= 32;
                if (c && oled_input_len < sizeof(oled_input_buf) - 1) {
                    oled_input_buf[oled_input_len++] = c;
                    oled_input_buf[oled_input_len]   = '\0';
                }
                return false;
            }
        }
    }

    return true;
}

/* ── OLED row 3 override during text input ───────────────── */
bool oled_task_user(void) {
    if (oled_input_active) {
        oled_set_cursor(0, 3);
        oled_write_P(PSTR(">"), false);
        oled_write(oled_input_buf, false);
        /* Blinking cursor */
        oled_write_char((timer_read() / 500) % 2 ? '_' : ' ', false);
        return false;   /* skip render_scroll */
    }
    return true;
}

/* ── VIA custom value: set OLED text from VIA app ────────── */
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] == 0x01 && data[1] == 0x01 && length >= 3) {
        uint8_t slen = data[2];
        if (slen > 62) slen = 62;
        char tmp[64] = {0};
        memcpy(tmp, &data[3], slen);
        oled_set_scroll_text(tmp);
    }
}
