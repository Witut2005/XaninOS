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
 
void screen_init(void)
{
    screen.cursor = VGA_TEXT_MEMORY;
    *screen.cursor = (uint16_t) 'c' + (((red << 4) | white) << 8);
    x = 0x0;
    y = 0x0;
}

void terminalKeyboard(uint8_t scanCode)
{

    keyboard_scan_code = scanCode;

    key_info.scan_code = scanCode;
    key_info.character = keyboard_map[scanCode];

    switch(key_info.scan_code)
    {
        case LSHIFT: {key_info.is_shift = true; return;}
        case LSHIFT_RELEASE: {key_info.is_shift = false; return;}
        case CAPS: {key_info.is_caps = ~key_info.is_caps; return;}
    }

    if(scanCode == CAPS)
    {
        if(caps_on)
            caps_on = false;
        else
            caps_on = true;

        return;
    }

    if(keyboard_scan_code >= 128 )
    {
        key_released = true;
        return;
    }

    key_released = false;
    

    if(!index)
    {
        for(int i = 0; i < 50;i++)
            keyboard_command[i] = '\0';
    }

    uint8_t key = keyboard_map[scanCode];

    if(caps_on || lshift_pressed)
        if(key >= 'a' && key <= 'z')
            key -= 32;

    if(lshift_pressed)
        if(key == '1' || key == '3' || key == '4' || key == '5')
            key -= 16;

    if(lshift_pressed)
    {
        remap_key('-','_');
        remap_key('2', '@');
        remap_key('6', '^');
        remap_key('7', '&');
        remap_key('8', '*');
        remap_key('9','(');
        remap_key('0',')');
        remap_key('=', '+');
        remap_key('[', '{');
        remap_key(']', '}');
        remap_key('/', '?');
        remap_key(';', ':');
        remap_key('`','~');
        remap_key(',','<');
        remap_key('.', '>');

        if(key == 0x27)
            key = 0x22;

    }

    if(arrows_navigate)
    {
        if(scanCode == ARROW_DOWN)
        {

            *cursor = (uint16_t)(selected_character | ((black << 4) | white) << 8);

            if((uint32_t)cursor <= VGA_TEXT_MEMORY + (80 * sizeof(uint16_t) * 27))           
                cursor += 79; 

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
                cursor -= 79; 
            
            selected_character = (char)*cursor;
            putchar_at_cursor('_');
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



    else if(scanCode == ARROW_UP || scanCode == ARROW_DOWN)
    {
        if(in_graphic_mode)
        {
            if(keyboard_scan_code == ARROW_UP)
                cursor -= 79;

            else if(keyboard_scan_code == ARROW_DOWN)
                cursor += 79;
        }

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

        if(print_off)
            return;

        *cursor = (uint16_t)(key | ((black << 4) | white) << 8); 
        keyboard_command[index] = key;
        index++;
        cursor++;
        x++;
    }


}