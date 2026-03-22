#pragma once

#include "quantum.h"

/* Set the scrolling text on the OLED bottom row (max 63 chars) */
void oled_set_scroll_text(const char *text);

/* Shared mute state between keyboard.c and keymap.c */
extern bool is_muted;
