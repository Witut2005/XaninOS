
#include <lib/stdiox.h>
#include <game_engine/xagame.h>

square* square_create(uint32_t x, uint32_t y, uint32_t size, uint8_t color)
{
    square* new_square = (square*)malloc(sizeof(square));
    

    new_square->position_x = x;
    new_square->position_y = y;
    new_square->size = size;
    new_square->color = color;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < (float)size * 1.5; j++)
            Screen.cursor[y][x + j] = ' ' | (set_output_color(color, white) << 8);

        y++;
    }

    return new_square;
}

rectangle* rectangle_create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color)
{
    rectangle* new_rectangle = (rectangle*) malloc(sizeof(rectangle));

    new_rectangle->position_x = x;
    new_rectangle->position_y = y;
    new_rectangle->size_x = size_x;
    new_rectangle->size_y = size_y;
    new_rectangle->color = color;


    for(int i = 0; i < new_rectangle->size_y; i++)
    {
        for(int j = 0; j < (float)new_rectangle->size_x * 1.5; j++)
            Screen.cursor[y + i][new_rectangle->position_x + j] = ' ' | (set_output_color(color, white) << 8);
    }

    return new_rectangle;

}

void rectangle_move(rectangle* rectangle, uint8_t x, uint8_t y)
{


    for(int i = 0; i < rectangle->size_y; i++)
    {
        for(int j = 0; j < (float)rectangle->size_x * 1.5; j++)
            Screen.cursor[rectangle->position_y + i][rectangle->position_x + j] = ' ' | (set_output_color(black, black) << 8);
    }

    rectangle->position_x += x;
    rectangle->position_y += y;

    for(int i = 0; i < rectangle->size_y; i++)
    {
        for(int j = 0; j < (float)rectangle->size_x * 1.5; j++)
            Screen.cursor[rectangle->position_y + i][rectangle->position_x + j] = ' ' | (set_output_color(rectangle->color, white) << 8);
    }    

}