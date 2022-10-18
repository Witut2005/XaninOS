

#pragma once

#include <stdint.h>
#include <libc/colors.h>

#define set_output_color(x,y) (x << 4 | (y & 0xF))

#define cursor_set_position(x_new, y_new) \
Screen.x = x_new; \
Screen.y = y_new

#define stderr ( red << 4 | white)


#define VRAM VGA_TEXT_MEMORY
#define VGA_SCREEN_RESOLUTION 4480

#define mkstr(str) #str

extern char* color_attributes[16];

#ifdef __cplusplus
extern "C" {
#endif

int screen_clear(void);
char putchar(char character);
char putchar_color(uint8_t color, char character);
void print_bcd_number(uint8_t x);
void xprintf(char* str, ... );
void xscanf(char* str, ... );
void xscan_range(char* string_buffer, uint32_t how_many_chars);

#define scanf xscanf
#define printf xprintf

#ifdef __cplusplus
}
#endif