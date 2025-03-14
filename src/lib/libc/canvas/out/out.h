#pragma once

#include <stdint.h>

void canvas_putchar(char character); // ONLY AVAILABLE IN CANVAS MODE
void canvas_putchar_color(uint8_t color, char character); // ONLY AVAILABLE IN CANVAS MODE
void canvas_screen_clear(void);
void canvas_xprintf(char* str, ...);
void canvas_screen_background_color_set(uint8_t color);