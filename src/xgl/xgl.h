

#pragma once

#include <stdint.h>

static inline uint8_t color(uint8_t given_color)
{
    return given_color >> 2;
}

void pixel_set(uint32_t x, uint32_t y, uint8_t given_color);
void rectangle_create(uint32_t x, uint32_t y, uint32_t x_size, uint32_t y_size, uint8_t given_color);
