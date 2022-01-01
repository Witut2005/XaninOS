
#pragma once


#include "./vty.h"
#include "../tetris/tetris.c"
#include "../xaninApps/help.c"
#include "../lib/signal.h"

void add_y(uint8_t yadd)
{
    cursor += yadd * 80;
    y++;
}

void set_y(uint8_t yset)
{
    cursor = (uint16_t*)VRAM + (yset * 80);
    y = yset;
}

static uint8_t index = 0x0;

void terminalKeyboard(uint8_t scanCode)
{


    if(index == 0x0)
    {
        for(int i = 0; i < 7;i++)
            COMMAND[i] = 0x0;
    }

    uint8_t key = keyboard_map[scanCode];

    if(scanCode == BSPC)
    {
        if(*(cursor-1) == (uint16_t)('>' | ((black << 4) | white) << 8))
            return;

        if(index != 0)
            index--;

        cursor--;
        *cursor = 0x0; //delete character
    }

    else if(scanCode == ENTER)
    {

        if(strlen(COMMAND) != 0)
        {

            cursor = (uint16_t*)VRAM;
            add_y(y);

            *cursor = (uint16_t)('>' | ((black << 4) | white) << 8); 
            cursor++;

            
        }

        index = 0x0;       

    }

    else
    {
        *cursor = (uint16_t)(key | ((black << 4) | white) << 8); 
        COMMAND[index] = key;
        index++;
        cursor++;
    }


}