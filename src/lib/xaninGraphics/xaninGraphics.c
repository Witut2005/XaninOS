
#include <lib/libc/string.h>
#include <lib/xaninGraphics/xaninGraphics.h>


void draw_line_x(uint8_t startx, uint8_t endx,uint8_t starty,uint8_t lineColor)
{
    uint16_t* lineCreator = (uint16_t*)0xb8000 + (starty * 80);


    for(uint8_t i = startx; i <= endx; i++)
    {
        lineCreator[i] = (uint16_t)(0x0 | (((lineColor << 4) | lineColor) << 8));
    }

}   

void draw_line_y(uint8_t starty, uint8_t endy,uint8_t startx,uint8_t lineColor)
{
    uint16_t* lineCreator = (uint16_t*)0xb8000 + startx + (starty * 80);

    for(uint8_t i = starty; i <= endy; i++)
    {
        *lineCreator = (uint16_t) ( 0x0 | (((lineColor << 4) | lineColor) << 8));
        lineCreator = lineCreator + 80;
    }

}

void draw_line_x_with_text(uint8_t startx, uint8_t endx,uint8_t starty,uint8_t lineColor, uint8_t text_color, char* text)
{
    uint16_t* lineCreator = (uint16_t*)0xb8000 + (starty * 80);


    for(uint8_t i = startx; i <= endx; i++)
        lineCreator[i] = (uint16_t)(0x0 | (((lineColor << 4) | text_color) << 8));
    
    for(int i = 0; i < strlen(text); i++)
        lineCreator[i] = (char)lineCreator[i] | text[i];

}   