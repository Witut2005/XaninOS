
#include <game_engine/xagame.hpp>
#include <libc/colors.h>


void xgm::rectangle::create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color)
{    

    this->is_destroyed = false;

    this->position_x = x;
    this->position_y = y;
    this->size_x = size_x;
    this->size_y = size_y;
    this->color = color;

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x + (size_x / 3); j++)
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(color, white) << 8);
    }
}



void xgm::rectangle::move(int32_t x, int32_t y)
{
    if(this->is_destroyed)
        return;

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x + (size_x / 3); j++)
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, white) << 8);

    }

    this->position_x += x;
    this->position_y += y;

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x + (size_x / 3); j++)
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color((this->color), white) << 8);
    }


}

void xgm::rectangle::destroy()
{
    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x + (size_x / 3); j++)
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, white) << 8);

    }

    is_destroyed = true;
}

void xgm::rectangle::rotate_right_90()
{
    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x + (size_x / 3); j++)
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, white) << 8);

    }

    uint32_t size_x_tmp = this->size_x;
    this->size_x = this->size_y;
    this->size_y = size_x_tmp;

    for(int i = 0; i < this->size_y - (size_y / 3); i++)
    {
        for(int j = 0; j < this->size_x + (size_x / 3); j++)
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color((this->color), white) << 8);
    }

}

xgm::rectangle::rectangle(uint32_t class_id)
{
    this->class_id = class_id;

}


xgm::ScreenManager::ScreenManager(void)
{
    this->screen_pointer = (uint8_t*)VGA_TEXT_MEMORY;
}

uint8_t& xgm::ScreenManager::operator[](uint32_t index)
{
    return this->screen_pointer[index * 2 + 1];
}


extern "C" void __cxa_pure_virtual()
{
    return;
}