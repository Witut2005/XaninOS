

#pragma once

#include <lib/libc/colors.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>

// #ifndef VGA_HEIGHT
// #define VGA_HEIGHT 25
// #endif

extern uint8_t logo_front_color;
extern uint8_t logo_back_color;

struct screen_t {
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;
};

typedef struct screen_t screen_t;

typedef uint8_t color_t;
typedef uint16_t vga_screen_cell_t;

extern screen_t Screen;

extern uint16_t** screen_rows;

void screen_init(void);
typedef void (*keyboard_handle_input)(void);

void terminal(void);
