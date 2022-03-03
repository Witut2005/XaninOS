
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

    keyboard_scan_code = scanCode;

    if(keyboard_scan_code == LSHIFT)
    {
        lshift_pressed = true;
        return;
    }

    if(keyboard_scan_code == LSHIFT_RELEASE)
    {
        lshift_pressed = false;
        return;
    }

    if(keyboard_scan_code >= 128 )
    {
        key_released = true;
        return;
    }

    key_released = false;

    if(print_off)
        return;

    if(!index)
    {
        for(int i = 0; i < 50;i++)
            keyboard_command[i] = '\0';
    }

    if(scanCode == CAPS)
    {
        if(caps_on)
            caps_on = false;
        else
            caps_on = true;

        return;
    }


    uint8_t key = keyboard_map[scanCode];

    if(caps_on || lshift_pressed)
        if(key >= 'a' && key <= 'z')
            key -= 32;

    if(lshift_pressed)
        if(key == '-')
            key = '_';

    if(arrows_navigate)
    {
        if(scanCode == ARROW_DOWN)
        {

            *cursor = (uint16_t)(selected_character | ((black << 4) | white) << 8);

            if((uint32_t)cursor <= VGA_TEXT_MEMORY + (80 * sizeof(uint16_t) * 27))           
                cursor += 80; 

            selected_character = (char)*cursor;
            putchar_at_cursor('_');
            return;
        }

        else if(scanCode == ARROW_RIGHT)
        {

            *cursor = (uint16_t)(selected_character | ((black << 4) | white) << 8);
            cursor++;

            selected_character = (char)*cursor;
            putchar_at_cursor('_');
            return;
        }

        else if(scanCode == ARROW_LEFT)
        {

            *cursor = (uint16_t)(selected_character | ((black << 4) | white) << 8);
            cursor--;

            selected_character = (char)*cursor;
            putchar_at_cursor('_');
            return;
        }

        else if(scanCode == ARROW_UP)
        {
            *cursor = (uint16_t)(selected_character | ((black << 4) | white) << 8);

            if((uint32_t)cursor >= VGA_TEXT_MEMORY + (80 * sizeof(uint16_t)))
                cursor -= 80; 
            
            selected_character = (char)*cursor;
            putchar_at_cursor('_');
            return;
        }
        
        
    }

    /*

    if(cursor_show)
    {
        selected_character = (char)*cursor;
        
        *cursor = (uint16_t)('_' | ((black << 4) | white) << 8);
    }

    */

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

    else if(scanCode == ARROW_UP || scanCode == ARROW_DOWN)
    {
        return;
    }

    else if(scanCode == ARROW_LEFT)
    {
        if(*(cursor-1) == (uint16_t)('>' | ((black << 4) | white) << 8))
            return;

        cursor--;
    }

    else if(scanCode == ARROW_RIGHT)
    {
        cursor++;
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
