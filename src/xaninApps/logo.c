
#include <terminal/vty.h>

void logo_color_change(char* color)
{
    logo_front_color = atoi(color) & 0x15;
    logo_back_color = atoi(color) >> 4;
}