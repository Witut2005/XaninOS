
#pragma once


#include <terminal/vty.h>
#include <tetris/tetris.c>
#include <xaninApps/help.c>
#include <lib/signal.h>

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

    if(!index)
    {
        for(int i = 0; i < 50;i++)
            keyboard_command[i] = '\0';
    }

    uint8_t key = keyboard_map[scanCode];

    if(arrows_navigate)
    {
        if(key == '2')
        {
            if((uint32_t)cursor <= VGA_TEXT_MEMORY + (80 * sizeof(uint16_t) * 27))           
                cursor += 80; 

            return;
        }

        else if(key == '6')
        {
            cursor++;
            return;
        }

        else if(key == '4')
        {
            cursor--;
            return;
        }

        else if(key == '8')
        {
            if((uint32_t)cursor >= VGA_TEXT_MEMORY + (80 * sizeof(uint16_t)))
                cursor -= 80; 
            return;
        }
        
        
    }

    if(scanCode == BSPC)
    {
        if(*(cursor-1) == (uint16_t)('>' | ((black << 4) | white) << 8))
            return;



        if(index != 0)
            index--;

        comBuf[index] = '\0';

        cursor--;
        *cursor = '\0'; /* delete character */
    }

    else if(scanCode == ENTER)
    {

        if(strlen(keyboard_command) != 0)
        {
            y++;
            x = 0;
            cursor = (uint16_t*)VRAM;
            add_y(y);

            if(!no_enter)
            {
                *cursor = (uint16_t)('>' | ((black << 4) | white) << 8); 
                cursor++;
                x++;
            }
            
        }

        index = 0x0;       

    }

    else
    {
        *cursor = (uint16_t)(key | ((black << 4) | white) << 8); 
        keyboard_command[index] = key;
        index++;
        cursor++;
        x++;
    }


}
