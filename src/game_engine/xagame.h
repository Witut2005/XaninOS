

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


square* square_create(uint32_t x, uint32_t y, uint32_t size, uint8_t color);
rectangle* rectangle_create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color);
void rectangle_move(rectangle* rectangle, uint8_t x, uint8_t y);
