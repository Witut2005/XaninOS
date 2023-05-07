#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>


uint16_t buf;
uint8_t colorTetris = 0x1;

uint16_t* petris_cursor = (uint16_t*)VGA_TEXT_MEMORY;


void tetris_input()
{


    if(KeyInfo.character == 'a')
        petris_cursor--;

    else if(KeyInfo.character == 'd')
        petris_cursor++;    

    else if(KeyInfo.character == '0')
        colorTetris = white;

    else if(KeyInfo.character == '1')
        colorTetris = blue;

    else if(KeyInfo.character == '2')
        colorTetris = green;

    else if(KeyInfo.character == '3')
        colorTetris = cyan;
        
    else if(KeyInfo.character == '4')
        colorTetris = red;

    else if(KeyInfo.character == '5')
        colorTetris = brown;

    else if(KeyInfo.character == '6')
        colorTetris = lblue;
        
    else if(KeyInfo.character == '7')
        colorTetris = yellow;

    else if(KeyInfo.character == '8')
        colorTetris = magenta;

    else if(KeyInfo.character == '9')
        colorTetris = lgray;

    if(KeyInfo.character == 'p')
    {
        *petris_cursor = buf;
        while(KeyInfo.character == 'p');

        *petris_cursor = 0x0;
    }

        
}