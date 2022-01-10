
#pragma once

#include <lib/signal.h>
#include <lib/hal.h>
#include <lib/stdiox.h>
#include <terminal/vty.c>
#include <headers/macros.h>
#include <keyboard/keyMap.h>
#include <handlers/handlers.h>




extern void pit_handler_init(void);
extern void keyboard_handler_init(void);



void floppy_interrupt(void)
{
    sprint(red,white,"FLOPPY ERROR");
    
    asm("cli");
    asm("hlt");
}

void invalid_opcode(void)
{
    xprintf("\n%zINVALID OPCODE",set_output_color(red,white));
    asm("cli");
    asm("hlt");
}

void divide_by_zero_exception(void)
{
    xprintf("\n%zPIT IRQ",set_output_color(red,white));
    asm("cli");
    asm("hlt");
}


uint8_t keyboard_scan_code;

bool pitActive = false; 

void pit_handler(void)
{
    pitActive = true;
}



void keyboard_handler(void)
{


    uint32_t zero = 0;


    keyStatus = inbIO(KEYBOARD_STATUS_REG); // if status & 1 (ON)
    keyboard_scan_code = inbIO(KEYBOARD_DATA_REG); // get keyboard_scan_code
    

    //HANDLE KEYBOARD APP SIGNAL 
    if(int32_63 & 0x2)
    {
        if(keyStatus & 1)
            keyboard_input = keyboard_map[keyboard_scan_code];

        signal.appHandler();
        return;
    }



    //PRINT CHARACTER

    else
    {

        if(keyStatus & 1)
        {

            if(keyboard_scan_code < 0 || keyboard_scan_code >= 128 )
                return;
        
            else
            {
                keyboard_input = keyboard_map[keyboard_scan_code];
                terminalKeyboard(keyboard_scan_code);
            }

        }
    
    return;

    }
}
 




