
#pragma once

#include <stdint.h>
#include <game_engine/xagame_types.h>


extern "C" square* square_create(uint32_t x, uint32_t y, uint32_t size, uint8_t color);
extern "C" rectangle* rectangle_create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color);
extern "C" void rectangle_move(rectangle* rectangle, uint8_t x, uint8_t y);
