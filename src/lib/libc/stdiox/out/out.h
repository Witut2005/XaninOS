#pragma once
#include <stdint.h>

#define OUTPUT_COLOR_SET(background, foreground) (((background)&0xF) << 4 | ((foreground)&0xF))
#define OUTPUT_POSITION_SET(y, x) ((((y)&0xFF) << 8) | ((x)&0xFF))

#define cursor_set_position(x_new, y_new) \
    Screen.x = x_new;                     \
    Screen.y = y_new

#define OUTPUT_COLOR_ERROR_SET (red << 4 | white)
#define SCREEN_CELL_SET_FROM_POINTER(ptr, background, foreground, character) *ptr = (uint16_t)((foreground | (background << 4)) << 8 | (character & 0xFF))

typedef uint8_t color_t;

#ifdef __cplusplus
extern "C" {
#endif

void putchar(char c);
void stdio_init(void);
void putchar_color(char c, color_t color);
int vga_screen_buffer_clear(void);
int screen_clear(void);
void putc(char* str, uint32_t count);
void putsc(char const* str, color_t color);
void puts(char const* str);
void puts_warning(char const* str);
void puts_error(char const* str);
void xprintf(char* fmt, ...);

#ifdef __cplusplus
}
#endif
