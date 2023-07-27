#pragma once

#include <stdint.h>
#include <lib/libc/colors.h>

// THIS LIBRARY WILL ONLY WORK WITH TEXT MODE TURNED ON

void draw_line_x(uint8_t startx, uint8_t endx,uint8_t starty,uint8_t lineColor);
void draw_line_y(uint8_t starty, uint8_t endy,uint8_t startx,uint8_t lineColor);
void draw_line_x_with_text(uint8_t startx, uint8_t endx,uint8_t starty,uint8_t lineColor, uint8_t text_color, char* text);


