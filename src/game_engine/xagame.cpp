
#include <game_engine/xagame.hpp>
#include <libc/colors.h>



xgm::CollisionInfo xgm::make_collision_info(bool x, uint8_t y, xgm::Direction z)
{
    xgm::CollisionInfo instance = {x,y,z};
    return instance;
}

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
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, black) << 8);

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

auto xgm::rectangle::size_get()
{
    return std::pair(this->size_x, this->size_y);
}

auto xgm::rectangle::position_get()
{
    return std::pair(this->position_x, this->position_y);
}

uint32_t xgm::rectangle::sizex_get()
{
    return this->size_x;
}

uint32_t xgm::rectangle::sizey_get()
{
    return this->size_y;
}

uint32_t xgm::rectangle::positionx_get()
{
    return this->position_x;
}

uint32_t xgm::rectangle::positiony_get()
{
    return this->position_y;
}

xgm::CollisionInfo xgm::rectangle::collision_detect()
{
    Renderer::ScreenManager XgmScreen;
    
    for(int i = 0; i < this->size_x; i++)
    {
        if(XgmScreen[(this->position_y - 1) * VGA_WIDTH + this->position_x + i] & 0xF0 != xgm::color::black)
            return xgm::make_collision_info(true, XgmScreen[(this->position_y - 1) * VGA_WIDTH + this->position_x + i], xgm::Direction::UP);
    }

    for(int i = 0; i < this->size_x; i++)
    {
        if(XgmScreen[(this->position_y + this->size_y) * VGA_WIDTH + this->position_x + i] & 0xF0 != xgm::color::black)
            return xgm::make_collision_info(true, XgmScreen[(this->position_y + this->size_y) * VGA_WIDTH + this->position_x + i], xgm::Direction::DOWN); 
    }

    for(int i = 0; i < this->size_y; i++)
    {
        if(XgmScreen[(this->position_x - 1) + (this->position_y + i) * VGA_WIDTH] != xgm::color::black)
            return xgm::make_collision_info(true, XgmScreen[(this->position_x - 1) + (this->position_y + i) + i], xgm::Direction::LEFT);
    }

    for(int i = 0; i < this->size_y; i++)
    {
        if(XgmScreen[(this->position_x + this->size_x) + (this->position_y + i) * VGA_WIDTH] != xgm::color::black)
            return xgm::make_collision_info(true, XgmScreen[(this->position_x + this->size_x) + (this->position_y + i) * VGA_WIDTH], xgm::Direction::RIGHT);
    }

    return xgm::make_collision_info(false, black, xgm::Direction::NONE);

}


xgm::Renderer::ScreenManager::ScreenManager(void)
{
    this->screen_pointer = (uint8_t*)VGA_TEXT_MEMORY;
}

uint8_t& xgm::Renderer::ScreenManager::operator[](uint32_t index)
{
    return this->screen_pointer[index * 2 + 1];
}

void xgm::Renderer::ScreenManager::vertical_line_create(uint8_t x, xgm::color::ColorAttributes color)
{       
    for(int i = 0; i < VGA_HEIGHT; i++)   
        (*this)[x + i * VGA_WIDTH] = static_cast<uint8_t> (color);
}

extern "C" void __cxa_pure_virtual()
{
    return;
}