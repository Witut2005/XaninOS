#pragma once
#include <stdint.h>

struct square
{
    uint32_t color;
    uint32_t position_x;
    uint32_t position_y;
    uint32_t size;
};

struct rectangle
{
    uint32_t color;
    uint32_t position_x;
    uint32_t position_y;
    uint32_t size_x;
    uint32_t size_y;  
};


typedef struct square square;
typedef struct rectangle rectangle;