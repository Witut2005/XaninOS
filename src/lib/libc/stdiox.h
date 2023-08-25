

#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/call/xanin_sys/calls/stdio/stdio.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>

#define OUTPUT_COLOR_SET(background, foreground) ((background & 0xF) << 4 | (foreground & 0xF))
#define OUTPUT_POSITION_SET(y, x) (((y & 0xFF) << 8) | (x & 0xFF))

#define cursor_set_position(x_new, y_new) \
Screen.x = x_new; \
Screen.y = y_new

#define stderr ( red << 4 | white)
#define ARRAY_LENGTH(x) SIZE_OF(x) / SIZE_OF(x[0])

enum SCREEN_MACROS 
{
    VGA_SCREEN_CELL_SIZE = 2
};

#define VGA_WIDTH (__vga_text_mode_width_get())
#define VGA_HEIGHT (__vga_text_mode_height_get())

enum STDIO_MODES{
    STDIO_MODE_CANVAS = 0,
    STDIO_MODE_TERMINAL = 1
};

enum XANIN_STANDARD_STREAMS{
    XANIN_STDOUT = 0,
    XANIN_STDIN = 1,
    XANIN_STDERR = 2
};


#define VGA_TEXT_MEMORY (__vga_buffer_segment_get())
#define VGA_SCREEN_RESOLUTION (VGA_WIDTH * VGA_HEIGHT)
#define BLANK_SCREEN_CELL 0
#define mkstr(str) #str
#define SCREEN_CELL_SET_FROM_POINTER(ptr, background, foreground, character) *ptr = (uint16_t)((foreground | (background << 4)) << 8 | (character & 0xFF))
#define VGA_MAX_Y (VGA_HEIGHT - 1)

extern char* color_attributes[16];

#ifdef __cplusplus
extern "C" {
#endif

int screen_clear(void);
int vga_screen_buffer_clear(void);
void putc(char* str, uint32_t count);
char putchar(char character);
char putchar_color(uint8_t color, char character);
void xprintf(char* str, ... );
void puts(const char* str);
void xscanf(char* str, ... );
void xscan_range(char* string_buffer, uint32_t how_many_chars);
void screen_background_color_set(color_t color);
void stdio_vty_set(Xtf* Front);
Xtf* stdio_vty_get(void);
void putst(const char* str);
void putct(const char* str, color_t color);

#ifdef __cplusplus
}
#endif