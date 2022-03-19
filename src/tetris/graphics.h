
#pragma once

#include <tetris/keybindings.h>


#define BLANK 0xa

uint8_t newInput;
uint8_t oldInput;


void printCube(void)
{
    x = 15*2;
    y = 1;

    petris_cursor = (uint16_t*)VGA_TEXT_MEMORY;

    petris_cursor += x * y;

    *petris_cursor = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
}


void update(void)
{

    buf = *petris_cursor;
    *petris_cursor = 0x0;

    tetris_input();



    if(((uint32_t)petris_cursor & 0xffffff00) == 0xB8B00)//VGA_TEXT_MEMORY + (25*80*2))
    {
        *petris_cursor = buf;            
        printCube();
    }


    petris_cursor = petris_cursor + 80;
    
    if((uint8_t)(*petris_cursor) == BLANK)
    {
        petris_cursor = petris_cursor - 80;
        *petris_cursor = (uint16_t)(BLANK | (((colorTetris << 4) | colorTetris) << 8));
        printCube();
    }

    buf = (uint16_t)((buf & 0x00ff) | (((colorTetris << 4) | colorTetris) << 8));  
    *petris_cursor = buf;    

	//sleep(1);
	
	/*
	
    for(int i = 0; i < 300;i++)
    {
        for(int j = 0; j < cpu_mhz;j++)
            asm("nop");
    }

	*/


}

