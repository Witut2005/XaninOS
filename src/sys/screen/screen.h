

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

extern screen_t Screen;

extern uint16_t** screen_rows;

void setTerminal();
void screen_init(void);

extern bool cursor_show;
extern char selected_character; 

#define putchar_at_cursor(x) *cursor = (uint16_t)(x | ((black << 4) | white) << 8)

#define remap_key(from, to)\
        if(key == from)\
            key = to


#define KEYBOARD_DRIVER_KEYBOARD_DRIVER_KEY_REMAP(from, to)\
        if(KeyInfo.character == from)\
            KeyInfo.character = to


typedef void (*keyboard_handle_input)(void);

extern keyboard_handle_input keyboard_handle;

extern char last_used_commands[40];
extern char last_used_parameters[40];

void terminal(void);
void letters_refresh(uint16_t* cursor_current_positon);
void letters_refresh_add(uint16_t* cursor_current_positon, char character_saved);
void keyboard_refresh_add(uint8_t keyboard_index_position, char character_saved);
void terminal_keyboard(void);
// char* app_exit_status_text_get(void);
