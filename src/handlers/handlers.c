
#pragma once

#include "../lib/signal.h"
#include "../lib/hal.h"
#include "../lib/stdiox.h"
#include "../terminal/vty.c"
#include "../headers/macros.h"
#include "../keyboard/keyMap.h"

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


static BYTE scanCode;

volatile bool pitActive = false; 

void pitHandler(void)
{
    pitActive = true;
}


void keyboardHandler(void)
{


    uint32_t zero = 0;


    uint8_t keyStatus = inbIO(KEYBOARD_STATUS_REG); // if status & 1 (ON)
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

            /*uint16_t* ptr;
            
    
            ptr = (unsigned short*)(0xb8000 + x) +  ((80)*y);

            if(scanCode == BSPC)
            {

                if(x == 0x2)
                    return;

                x -= 0x2;
                ptr = (unsigned short*)(0xb8000 + x) +  ((80)*y);
                *ptr = (uint16_t)input | (((black << 4) | black) << 8); 
                
                if(index != 0)
                    index--;
            }

            else if(scanCode == ENTER)
            {

                
                y++;
                x = 0x0;
                sprint(black,white,">");
                x = 0x2;
                y--;

                

                index = 0x0;

            }

            else
            {
                if(index == 0x0)
                {
                    for(int i = 0; i < 7;i++)
                        COMMAND[i] = 0x0;
                }
                
                *ptr = (uint16_t)input | (((black << 4) | white) << 8);
                COMMAND[index] = input;
                index++;
                x += 2;
            }
        */
        }

    }
    
    return;

    }
}
 




void kbInit() 
{
    // irq sa active low tzn 0 = true

	outbIO(0x64, 0xAE); // klawa wlaczona
    outbIO(PIC1_DATA_REG, 0xFD); // 11111101 <-- irq1 wlaczone 
    //sprint(red,white,Ncum);
}
