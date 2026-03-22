#include "quantum.h"
#include "multi_function_keypad.h"

/* ── Mute state ─────────────────────────────────────────────── */
bool is_muted = false;

/* ════════════════════════════════════════════════════════════
 *  RGB MATRIX – physical LED positions
 *  2 SK6812MINI LEDs:
 *    LED 0 = D2  (first in chain: GP0 → D2 → D1)
 *    LED 1 = D1  (second in chain)
 *  Approximate positions on PCB (QMK units: 0-224 x, 0-64 y)
 * ════════════════════════════════════════════════════════════ */
#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {
    /* Key matrix → LED index
       No LED is tied to a specific key (underglow only) */
    {
        { NO_LED, NO_LED, NO_LED, NO_LED }
    },
    /* Physical positions */
    {
        { 56,  32 },   /* D2 */
        { 168, 32 }    /* D1 */
    },
    /* Underglow flags */
    {
        LED_FLAG_UNDERGLOW,
        LED_FLAG_UNDERGLOW
    }
};
#endif

/* ════════════════════════════════════════════════════════════
 *  OLED SSD1306 128×64
 *  4-row layout (6×8 font, 21 columns):
 *    Row 0 – Fixed title
 *    Row 1 – Active layer
 *    Row 2 – Volume / mute status
 *    Row 3 – Custom scrolling text
 * ════════════════════════════════════════════════════════════ */
#ifdef OLED_ENABLE

static char     scroll_buf[64] = OLED_SCROLL_TEXT;
static uint16_t scroll_offset  = 0;
static uint8_t  scroll_timer   = 0;

void oled_set_scroll_text(const char *text) {
    strncpy(scroll_buf, text, sizeof(scroll_buf) - 1);
    scroll_buf[sizeof(scroll_buf) - 1] = '\0';
    scroll_offset = 0;
    scroll_timer  = 0;
}

/* ── Row 0: title ────────────────────────────────────────── */
static void render_header(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR(" Multi  Keypad  "), false);
}

/* ── Row 1: active layer ─────────────────────────────────── */
static void render_layer(void) {
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("Layer: "), false);
    oled_write_char('0' + get_highest_layer(layer_state), false);
    oled_write_P(PSTR("              "), false);
}

/* ── Row 2: volume status ────────────────────────────────── */
static void render_volume(void) {
    oled_set_cursor(0, 2);
    if (is_muted) {
        oled_write_P(PSTR("Vol:  [MUTE]    "), false);
    } else {
        oled_write_P(PSTR("Vol:  active    "), false);
    }
}

/* ── Row 3: scrolling text ───────────────────────────────── */
#define OLED_COLS 21

static void render_scroll(void) {
    oled_set_cursor(0, 3);

    uint8_t len = (uint8_t)strlen(scroll_buf);
    if (len == 0) return;

    if (len <= OLED_COLS) {
        /* Short text: centered and static */
        uint8_t pad = (OLED_COLS - len) / 2;
        for (uint8_t i = 0; i < pad; i++)
            oled_write_char(' ', false);
        oled_write(scroll_buf, false);
        for (uint8_t i = 0; i < OLED_COLS - len - pad; i++)
            oled_write_char(' ', false);
    } else {
        /* Long text: scroll right to left */
        uint8_t total = len + 3;  /* text + 3 spaces separator */

        char visible[OLED_COLS + 1];
        for (uint8_t i = 0; i < OLED_COLS; i++) {
            uint8_t idx = (scroll_offset + i) % total;
            visible[i]  = (idx < len) ? scroll_buf[idx] : ' ';
        }
        visible[OLED_COLS] = '\0';
        oled_write(visible, false);

        scroll_timer++;
        if (scroll_timer >= OLED_SCROLL_SPEED) {
            scroll_timer  = 0;
            scroll_offset = (scroll_offset + 1) % total;
        }
    }
}

/* ── Main OLED task ──────────────────────────────────────── */
bool oled_task_kb(void) {
    if (!oled_task_user()) return false;
    render_header();
    render_layer();
    render_volume();
    render_scroll();
    return false;
}

#endif /* OLED_ENABLE */
