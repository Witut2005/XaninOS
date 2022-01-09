#pragma once

#include <stdint.h>
#include "../headers/macros.h"
#include "../headers/colors.h"


uint16_t buf;
uint8_t colorTetris = 0x1;

uint16_t* petris_cursor = (uint16_t*)VGA_TEXT_MEMORY;


void tetris_input()
{

    asm("int 0x21");

    if(keyboard_input == 'a')
        petris_cursor--;

    else if(keyboard_input == 'd')
        petris_cursor++;    

    else if(keyboard_input == '0')
        colorTetris = white;

    else if(keyboard_input == '1')
        colorTetris = blue;

    else if(keyboard_input == '2')
        colorTetris = green;

    else if(keyboard_input == '3')
        colorTetris = cyan;
        
    else if(keyboard_input == '4')
        colorTetris = red;

    else if(keyboard_input == '5')
        colorTetris = brown;

    else if(keyboard_input == '6')
        colorTetris = lblue;
        
    else if(keyboard_input == '7')
        colorTetris = yellow;

    else if(keyboard_input == '8')
        colorTetris = magenta;

    else if(keyboard_input == '9')
        colorTetris = lgray;

    if(keyboard_input == 'p')
    {
        *petris_cursor = buf;
        while(keyboard_input == 'p');

        *petris_cursor = 0x0;
    }

        
}