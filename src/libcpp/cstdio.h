
#pragma once

#include <stdint.h>
#include <libc/colors.h>

#define stderr (white << 4 | red)
#define stdok (white << 4 | green)


extern "C"
{


void screen_clear(void);
char putchar(char character);
void print_bcd_number(uint8_t x);
void xprintf(char* str, ... );
void xscanf(char* str, ... );
void xscan_range(char* string_buffer, uint32_t how_many_chars);

}