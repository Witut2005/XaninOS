

#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>

#define OUTPUT_COLOR_SET(background, foreground) ((background & 0xF) << 4 | (foreground & 0xF))
#define OUTPUT_POSITION_SET(y, x) (((y & 0xFF) << 8) | (x & 0xFF))

#define cursor_set_position(x_new, y_new) \
Screen.x = x_new; \
Screen.y = y_new

#define stderr ( red << 4 | white)
#define ARRAY_LENGTH(x) sizeof(x) / sizeof(x[0])

enum screen_macros
{
    VGA_WIDTH = 80,
    VGA_HEIGHT = 25,
    VGA_SCREEN_CELL_SIZE = 2
};

#define VRAM 0xb8000
#define VGA_TEXT_MEMORY 0xb8000
#define VGA_SCREEN_RESOLUTION (25 * 80)
#define BLANK_SCREEN_CELL 0
#define mkstr(str) #str
#define SCREEN_CELL_SET_FROM_POINTER(ptr, background, foreground, character) *ptr = (uint16_t)((foreground | (background << 4)) << 8 | (character & 0xFF))

extern char* color_attributes[16];

#ifdef __cplusplus
extern "C" {
#endif

int screen_clear(void);
void putc(char* str, uint32_t count);
char putchar(char character);
char putchar_color(uint8_t color, char character);
void print_bcd_number(uint8_t x);
void xprintf(char* str, ... );
void xscanf(char* str, ... );
void xscan_range(char* string_buffer, uint32_t how_many_chars);
void screen_background_color_set(uint8_t color);

#define scanf xscanf
#define printf xprintf

#ifdef __cplusplus
}
#endif