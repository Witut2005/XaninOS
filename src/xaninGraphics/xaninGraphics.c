#pragma once

#include <xaninGraphics/xaninGraphics.h>


void drawLineX(uint8_t startx, uint8_t endx,uint8_t starty,uint8_t lineColor)
{
    uint16_t* lineCreator = (uint16_t*)0xb8000 + (starty * 80);


    for(uint8_t i = startx; i <= endx; i++)
    {
        lineCreator[i] = (uint16_t)(0x0 | (((lineColor << 4) | lineColor) << 8));
    }

}   

void drawLineY(uint8_t starty, uint8_t endy,uint8_t startx,uint8_t lineColor)
{
    uint16_t* lineCreator = (uint16_t*)0xb8000 + startx + (starty * 80);

    for(uint8_t i = starty; i <= endy; i++)
    {
        *lineCreator = (uint16_t) ( 0x0 | (((lineColor << 4) | lineColor) << 8));
        lineCreator = lineCreator + 80;
    }

}
