
#pragma once

#include <libc/hal.h>
#include <terminal/vty.h>
#include <devices/APIC/apic_registers.h>

extern void screenshot(void);

void keyboard_driver(uint8_t scanCode)
{
    if(KeyInfo.scan_code == scanCode)
        KeyInfo.is_hold = true;
        
    else
        KeyInfo.is_hold = false;


    KeyInfo.scan_code = scanCode;


    switch(KeyInfo.scan_code)
    {
        case LSHIFT                       : {KeyInfo.is_shift = true; KeyInfo.character = '\0'; break;}
        case LSHIFT_RELEASE               : {KeyInfo.is_shift = false; break;}
        case BSPC                         : 
        {
            KeyInfo.is_bspc               = true;
            
            if(keyboard_handle != nullptr)
                keyboard_handle();
            break;            
        }
        
        case BSPC_RELEASE                 : {KeyInfo.is_bspc = false; break;}
        case CAPS                         : 
        {
            KeyInfo.is_caps               = KeyInfo.is_caps  ?  false : true; 
            break;
        }

        case ARROW_UP                     : {KeyInfo.is_up = true;KeyInfo.character = 0x0; break;}
        case ARROW_UP_RELEASE             : {KeyInfo.is_up = false; break;}

        case ARROW_DOWN                   : {KeyInfo.is_down = true; KeyInfo.character = 0x0;break;}
        case ARROW_DOWN_RELEASE           : {KeyInfo.is_down = false; break;}

        case ARROW_RIGHT                  : {KeyInfo.is_right = true; KeyInfo.character = 0x0;break;}
        case ARROW_RIGHT_RELEASE          : {KeyInfo.is_right = false; break;}

        case ARROW_LEFT                   : {KeyInfo.is_left = true; KeyInfo.character = 0x0; break;}
        case ARROW_LEFT_RELEASE           : {KeyInfo.is_left = false; break;}
        case PRINT_SCREEN_KEY             : 
        {
            int x_tmp                     = Screen.x, y_tmp = Screen.y;
            screenshot(); 
            *(uint32_t*)APIC_EOI_REGISTER = 0x0; 
            eoi_send(); 
            KeyInfo.character             = 0x0; 
            Screen.x                      = x_tmp;
            Screen.y                      = y_tmp;

            break;
            
        }

        case L_ALT                        : {KeyInfo.is_alt = true; break;}
        case L_ALT_RELEASE                : {KeyInfo.is_alt = false; break;}

        case L_CTRL                       : {KeyInfo.is_ctrl = true; break;}
        case L_CTRL_RELEASE               : {KeyInfo.is_ctrl = false; break;}
        case F4_KEY                       : {KeyInfo.character = 0x0; break;}
        case F4_KEY_RELEASE               : {KeyInfo.character = 0x0; break;}
    

    }

    KeyInfo.character = keyboard_map[scanCode];

    if(KeyInfo.scan_code >= 128)
    {
        KeyInfo.is_pressed = false;
        KeyInfo.character = 0x0;
    }

    else
    {
        KeyInfo.is_pressed = true;
    }

    if(KeyInfo.scan_code == BSPC)
    {
        KeyInfo.character = 0x0;
    }

    if(KeyInfo.is_caps) 
    {
        if(KeyInfo.character >= 'a' && KeyInfo.character <= 'z')
        {
            KeyInfo.character -= 32;
        }
    }

    if(KeyInfo.is_shift)
    {
        if(KeyInfo.is_caps)
        {
            if(KeyInfo.character >= 'A' && KeyInfo.character <= 'Z')
            {
                KeyInfo.character += 32;
            }
        }

        else
        {
            if(KeyInfo.character >= 'a' && KeyInfo.character <= 'z')
            {
                KeyInfo.character -= 32;
            }
        
        }
    }

    if(KeyInfo.is_shift)
    {
        key_remap('-','_');
        key_remap('1','!');
        key_remap('2', '@');
        key_remap('3','#');
        key_remap('4', '$');
        key_remap('5', '%');
        key_remap('6', '^');
        key_remap('7', '&');
        key_remap('8', '*');
        key_remap('9','(');
        key_remap('0',')');
        key_remap('=', '+');
        key_remap('[', '{');
        key_remap(']', '}');
        key_remap('/', '?');
        key_remap(';', ':');
        key_remap('`','~');
        key_remap(',','<');
        key_remap('.', '>');
        key_remap('/', '?');
        key_remap(0x5C, '|');
        key_remap(0x27, 0x22);

    }


    if(keyboard_handle != nullptr)
    {
        keyboard_handle();
    }
        
    if(KeyInfo.scan_code == LSHIFT || KeyInfo.scan_code == CAPS)
        KeyInfo.character = '\0';

    *(uint32_t*)APIC_EOI_REGISTER = 0x0;

}



