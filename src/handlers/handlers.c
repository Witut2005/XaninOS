
#pragma once

#include <lib/signal.h>
#include <lib/hal.h>
#include <lib/stdiox.h>
#include <terminal/vty.c>
#include <headers/macros.h>
#include <keyboard/keyMap.h>
#include <keyboard/keyboard_driver.c>
#include <handlers/handlers.h>

//#include <devices/DMA/dma.c>



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
    xprintf("\n%DIVIDE BY ZERO ERROR",set_output_color(red,white));
    asm("cli");
    asm("hlt");
}





void pit_handler(void)
{
    pitActive = true;

    pit_current_time++;

}



void keyboard_handler(void)
{


    uint32_t zero = 0;


    keyStatus = inbIO(KEYBOARD_STATUS_REG); // if status & 1 (ON)
    keyboard_scan_code = inbIO(KEYBOARD_DATA_REG); // get keyboard_scan_code

    keyboard_driver(keyboard_scan_code);

}
 


