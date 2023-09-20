
#pragma once

#include <stdint.h>

extern uint16_t* print_ptr;
extern uint32_t y;
extern uint32_t x;

void print(const char* str);
void print_decimal(uint32_t value);
void print_hex(uint32_t value);
void vga_screen_buffer_clear(void);