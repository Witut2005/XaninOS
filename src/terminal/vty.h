

#pragma once

#include <headers/macros.h>
#include <headers/colors.h>
#include <keyboard/keyMap.h>
#include <lib/stdiox.h>
#include <lib/string.h>

#define SCREEN_WIDTH 25
#define SCREEN_HEIGHT 80
#define SCREEN_RESOLUTION (25 * 80)

#define VGA_TEXT_MEMORY 0xb8000

uint16_t* cursor = (uint16_t*)VGA_TEXT_MEMORY + (4*80);



static uint8_t y,x;

void setTerminal();

void set_x(uint8_t newVal);

void set_y(uint8_t newVal);

void add_x(uint8_t xadd);


void add_y(uint8_t yadd);

bool no_enter = false;

bool arrows_navigate = false;
bool terminalAppExit = false;
bool cursor_show = false;
char selected_character; 

#define putchar_at_cursor(x) *cursor = (uint16_t)(x | ((black << 4) | white) << 8)

#define remap_key(from, to)\
        if(key == from)\
            key = to


bool caps_on = false;
bool print_off = false;
bool key_released = false;
bool lshift_pressed = false;

bool in_graphic_mode = false;

