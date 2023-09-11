
#pragma once

#include <programs/tetris/keybindings.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>

#define BLANK 0xa

static uint8_t newInput;
static uint8_t oldInput;

static uint8_t x;
static uint8_t y;

void printCube(void)
{
    x = 15*2;
    y = 1;

    petris_cursor = (uint16_t*)VGA_TEXT_MEMORY;

    petris_cursor += x * y;

    *petris_cursor = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
}


void update(void)
{

    buf = *petris_cursor;
    *petris_cursor = 0x0;

    tetris_input();



    if((uint32_t)petris_cursor >= VGA_TEXT_MEMORY + (27 * 80 * 2))//VGA_TEXT_MEMORY + (25*80*2))
    {
        *petris_cursor = buf;            
        printCube();
    }


    petris_cursor = petris_cursor + 80;
    
    if((uint8_t)(*petris_cursor) == BLANK)
    {
        petris_cursor = petris_cursor - 80;
        *petris_cursor = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
        printCube();
    }

    buf = (uint16_t)((buf & 0x00ff) | (((colorTetris << 4) | colorTetris) << 8));  
    *petris_cursor = buf;    

    msleep(300);
}
