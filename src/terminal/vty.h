

#pragma once

#include <stdint.h>
#include <stddef.h>

#define nullptr 0x0



#define SCREEN_WIDTH 28
#define SCREEN_HEIGHT 80
#define SCREEN_RESOLUTION (25 * 80)

enum screen_macros
{
    VGA_WIDTH = 80,
    VGA_HEIGHT = 28
};

#define VGA_TEXT_MEMORY 0xb8000


uint8_t logo_front_color = magenta;
uint8_t logo_back_color = black;

struct key_info_t
{
    uint8_t scan_code;
    char character;

    bool is_shift;
    bool is_ctrl;
    bool is_caps;
    bool is_bspc;
    bool is_alt;

    bool is_left;
    bool is_right;
    bool is_up;
    bool is_down;
    
    bool is_pressed;
    bool is_hold;

};

typedef struct key_info_t key_info_t;
key_info_t KeyInfo;

struct screen_t
{
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;

};

typedef struct screen_t screen_t;

screen_t Screen;

uint16_t* screen_buffer[60];
uint8_t index = 0x0;
char character_blocked = '\0';

static uint8_t y,x;
void setTerminal();
void screen_init(void);

bool no_enter = false;

bool arrows_navigate = false;
bool terminalAppExit = false;
bool cursor_show = false;
char selected_character; 

#define putchar_at_cursor(x) *cursor = (uint16_t)(x | ((black << 4) | white) << 8)

#define remap_key(from, to)\
        if(key == from)\
            key = to


#define key_remap(from, to)\
        if(KeyInfo.character == from)\
            KeyInfo.character = to

bool caps_on = false;
bool print_off = false;
bool key_released = false;
bool lshift_pressed = false;
bool in_graphic_mode = false;

typedef void (*keyboard_handle_input)(void);

keyboard_handle_input keyboard_handle = nullptr;
 
char program_name[40];
char program_parameters[40];
char program_parameters1[40];
char program_parameters2[40];
char program_parameters3[40];

char* argv[5];

char last_used_commands[40];
char last_used_parameters[40];


void terminal(void);
void letters_refresh(uint16_t* cursor_current_positon);
void letters_refresh_add(uint16_t* cursor_current_positon, char character_saved);
void keyboard_refresh_add(uint8_t keyboard_index_position, char character_saved);
void terminal_keyboard(void);
