
#pragma once

#include <stdint.h>

extern "C"
{

void screen_clear(void);
char putchar(char character);
void print_bcd_number(uint8_t x);
void xprintf(char* str, ... );
void xscanf(char* str, ... );

}