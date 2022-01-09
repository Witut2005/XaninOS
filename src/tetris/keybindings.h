#pragma once

#include <stdint.h>
#include "../headers/macros.h"
#include "../headers/colors.h"


uint16_t buf;
uint8_t colorTetris = 0x1;

void inputTetris()
{

    asm("int 0x21");

    if(input == 'a')
        petrisCursor--;

    else if(input == 'd')
        petrisCursor++;    

    else if(input == '0')
        colorTetris = white;

    else if(input == '1')
        colorTetris = blue;

    else if(input == '2')
        colorTetris = green;

    else if(input == '3')
        colorTetris = cyan;
        
    else if(input == '4')
        colorTetris = red;

    else if(input == '5')
        colorTetris = brown;

    else if(input == '6')
        colorTetris = lblue;
        
    else if(input == '7')
        colorTetris = yellow;

    else if(input == '8')
        colorTetris = magenta;

    else if(input == '9')
        colorTetris = lgray;

    if(input == 'p')
    {
        *petrisCursor = buf;
        while(input == 'p');

        *petrisCursor = 0x0;
    }

        
}