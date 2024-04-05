

#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>
#include <sys/call/xanin_sys/calls/stdio/stdio.h>

#define OUTPUT_COLOR_SET(background, foreground) (((background) & 0xF) << 4 | ((foreground) & 0xF))
#define OUTPUT_POSITION_SET(y, x) ((((y) & 0xFF) << 8) | ((x) & 0xFF))

#define cursor_set_position(x_new, y_new) \
Screen.x = x_new; \
Screen.y = y_new

#define OUTPUT_COLOR_ERROR_SET ( red << 4 | white)
#define mkstr(str) #str
#define SCREEN_CELL_SET_FROM_POINTER(ptr, background, foreground, character) *ptr = (uint16_t)((foreground | (background << 4)) << 8 | (character & 0xFF))

#ifdef __cplusplus
extern "C" {
#endif

    void stdio_init(void);
    int screen_clear(void);
    int vga_screen_buffer_clear(void);

    void putchar(char c);
    void putchar_color(char c, color_t color);

    void xprintf(char* str, ...);
    void new_xprintf(char* fmt, ...);
    void xscanf(char* str, ...);
    void xscan_range(char* string_buffer, uint32_t how_many_chars);
    void stdio_vty_set(Xtf* Front);
    Xtf* stdio_vty_get(void);

    void putc(char* str, uint32_t count);
    void putsc(const char* str, color_t color);
    void puts(const char* str);
    void puts_warning(const char* str);
    void puts_error(const char* str);

    void stdio_refresh(address_t* args);

#ifdef __cplusplus
}
#endif