

#ifndef VTY_H
#define VTY_H

#include "../headers/macros.h"
#include "../headers/colors.h"
#include "../keyboard/keyMap.h"
#include "../lib/stdiox.h"
#include "../lib/string.h"

#define SCREEN_WIDTH 25
#define SCREEN_HEIGHT 80
#define SCREEN_RESOLUTION (25 * 80)

#define VRAM 0xb8000

uint16_t* cursor = (uint16_t*)VRAM + (4*80);

static uint8_t y,x;

void setTerminal();

void set_x(uint8_t newVal);

void set_y(uint8_t newVal);

void add_x(uint8_t xadd);


void add_y(uint8_t yadd);

bool no_enter = false;

bool terminalAppExit = false;

#endif
