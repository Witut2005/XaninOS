

#pragma once

#include <stdint.h>
#include <sys/devices/vga/vga.h>


void xgl_init(xgm_t mode);
uint8_t color(uint8_t given_color);
void pixel_set(uint32_t x, uint32_t y, uint8_t given_color);
void rectangle_create(uint32_t x, uint32_t y, uint32_t x_size, uint32_t y_size, uint8_t given_color);
void line_horizontal_create(uint32_t x, uint32_t y, uint16_t lenght, uint8_t given_color);
uint8_t rgb2vga(int r, int g, int b);
// void circle_create(uint32_t x, uint32_t y, uint32_t radius, uint8_t given_color);
