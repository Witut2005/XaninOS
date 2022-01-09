
#pragma once

#include "./keybindings.h"


#define BLANK 0xa

uint8_t newInput;
uint8_t oldInput;



void printCube(void)
{
    x = 15*2;
    y = 1;

    petrisCursor = (uint16_t*)VGA_TEXT_MEMORY;

    petrisCursor += x * y;

    *petrisCursor = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
}


void update(void)
{

    buf = *petrisCursor;
    *petrisCursor = 0x0;

    inputTetris();



    if(((uint32_t)petrisCursor & 0xffffff00) == 0xB8B00)//VGA_TEXT_MEMORY + (25*80*2))
    {
        *petrisCursor = buf;            
        printCube();
    }


    petrisCursor = petrisCursor + 80;
    
    if((uint8_t)(*petrisCursor) == BLANK)
    {
        petrisCursor = petrisCursor - 80;
        *petrisCursor = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
        printCube();
    }

    buf = (uint16_t)((buf & 0x00ff) | (((colorTetris << 4) | colorTetris) << 8));  
    *petrisCursor = buf;    


    for(int i = 0; i < 300;i++)
    {
        for(int j = 0; j < cpu_mhz;j++)
            asm("nop");
    }



}

