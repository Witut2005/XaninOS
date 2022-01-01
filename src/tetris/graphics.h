
#pragma once

#include "./keybindings.h"


#define BLANK 0xa

uint8_t newInput;
uint8_t oldInput;



void printCube(void)
{
    x = 15*2;
    y = 1;

    scrPtr = (uint16_t*)0xb8000;

    scrPtr += x * y;

    *scrPtr = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
}


void update(void)
{

    buf = *scrPtr;
    *scrPtr = 0x0;

    inputTetris();



    if(((uint32_t)scrPtr & 0xffffff00) == 0xB8B00)//0xb8000 + (25*80*2))
    {
        *scrPtr = buf;            
        printCube();
    }


    scrPtr = scrPtr + 80;
    
    if((uint8_t)(*scrPtr) == BLANK)
    {
        scrPtr = scrPtr - 80;
        *scrPtr = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
        printCube();
    }

    buf = (uint16_t)((buf & 0x00ff) | (((colorTetris << 4) | colorTetris) << 8));  
    *scrPtr = buf;    


    for(int i = 0; i < 300;i++)
    {
        for(int j = 0; j < cpu_mhz;j++)
            asm("nop");
    }



}

