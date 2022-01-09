
#pragma once

#include "../lib/signal.h"
#include "../lib/hal.h"
#include "../lib/stdiox.h"
#include "../terminal/vty.c"
#include "../headers/macros.h"
#include "../keyboard/keyMap.h"
#include "./handlers.h"




extern void pitHandlerInit(void);
extern void keyboardHandlerInit(void);


void floppyIrq(void)
{
    sprint(red,white,"FLOPPY ERROR");
    
    asm("cli");
    asm("hlt");
}

void invalidOpcode(void)
{
    xprintf("\n%zINVALID OPCODE",getColors(red,white));
    asm("cli");
    asm("hlt");
}

void divError(void)
{
    xprintf("\n%zPIT IRQ",getColors(red,white));
    asm("cli");
    asm("hlt");
}


uint8_t scanCode;

bool pitActive = false; 

void pitHandler(void)
{
    pitActive = true;
}



void keyboardHandler(void)
{


    uint32_t zero = 0;


    keyStatus = inbIO(KEYBOARD_STATUS_REG); // if status & 1 (ON)
    scanCode = inbIO(KEYBOARD_DATA_REG); // get scanCode
    

    //HANDLE KEYBOARD APP SIGNAL 
    if(int32_63 & 0x2)
    {
        if(keyStatus & 1)
            input = keyboard_map[scanCode];

        signal.appHandler();
        return;
    }



    //PRINT CHARACTER

    else
    {

        if(keyStatus & 1)
        {

            if(scanCode < 0 || scanCode >= 128 )
                return;
        
            else
            {
                input = keyboard_map[scanCode];
                terminalKeyboard(scanCode);
            }

        }
    
    return;

    }
}
 




