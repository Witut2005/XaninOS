
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <lib/libc/colors.h>

#define OUTPUT_COLOR_SET(background, foreground) ((background & 0xF) << 4 | (foreground & 0xF))
#define VGA_COLOR(x) (x << 8)
#define BOOTIO_NO_COLOR 0

typedef uint16_t vga_cell_t;
typedef uint8_t color_t;

extern uint16_t* print_ptr;
extern uint32_t y;
extern uint32_t x;

void putchar(char c, color_t color);
void print(const char* format, ...);
void print_decimal(uint32_t value, color_t color);
void print_hex(uint32_t value, color_t color);
void vga_screen_buffer_clear(void);