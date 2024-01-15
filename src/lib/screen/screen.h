

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <lib/libc/colors.h> 
#include <sys/call/xanin_sys/calls/vga/vga.h>


// #ifndef VGA_HEIGHT
// #define VGA_HEIGHT 25
// #endif

extern uint8_t logo_front_color;
extern uint8_t logo_back_color;


struct screen_t
{
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

extern keyboard_handle_input keyboard_handle;

void terminal(void);
void letters_refresh (uint16_t* cursor_current_positon) __attribute__((deprecated));
void letters_refresh_add (uint16_t* cursor_current_positon, char character_saved) __attribute__((deprecated));
void keyboard_refresh_add (uint8_t keyboard_index_position, char character_saved) __attribute__((deprecated));
