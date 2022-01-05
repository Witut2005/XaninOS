
#pragma once

#include "../lib/signal.h"
#include "../lib/hal.h"
#include "../lib/stdiox.h"
#include "../terminal/vty.c"
#include "../headers/macros.h"
#include "../keyboard/keyMap.h"
#include "./handlers.h"

#define PIC1_COMMAND_REG 0x20
#define PIC1_STATUS_REG 0x20

#define PIC1_DATA_REG 0x21    
#define PIC1_MASK_REG 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_STATUS_REG 0xA0

#define PIC2_DATA 0xA1    
#define PIC_MASK_REG 0xA1

#define KEYBOARD_ENCODER 0x60
#define ONBOARD_KEY_CONTROLLER 0x64
#define KEYBOARD_STATUS_REG 0x64

#define KEYBOARD_DATA_REG 0x60




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
    sprint(red,white,"INVALID OPCODE");
    asm("cli");
    asm("hlt");
}

void divError(void)
{
    sprint(red,white,"PIT IRQ");
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
 




void kbInit() 
{
	outbIO(0x64, 0xAE);             // KEYBOARD ON
    outbIO(PIC1_DATA_REG, 0xFD);    // 11111101 <-- irq1 ON 
}
