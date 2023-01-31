
#include <game_engine/xagame.hpp>
#include <libc/colors.h>
#include <libc/stdlibx.h>


extern "C" void printk(char* str);

bool xgm::Renderer::ScreenManager::screen_cells[VGA_HEIGHT][VGA_WIDTH] = {false};

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

    if(BlankCells != nullptr)
    {
        for(int i = 0; i < this->size_y; i++)
            free(BlankCells[i]);

        free(BlankCells);
    }

    BlankCells = (bool**)calloc(sizeof(bool*) * this->size_y);

    for(int i = 0; i < this->size_y; i++)
        BlankCells[i] = (bool*)calloc(sizeof(bool) * this->size_x);

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < size_x; j++)
        {
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(color, white) << 8);
            xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = true;
        }
    }

}

void xgm::rectangle::move(int32_t x, int32_t y)
{
    if(this->is_destroyed)
        return;

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x; j++)
        {
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, black) << 8);
            xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = false;
        }
    }

    this->position_x += x;
    this->position_y += y;

    for(int i = 0; i < this->size_y; i++)
    {
        bool dont_copy_me = false;
        for(int j = 0; j < this->size_x; j++)
        {
            if(BlankCells[i][j])
                dont_copy_me = true;

            if(!dont_copy_me)
            {
                Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color((this->color), white) << 8);
                xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = true;
            }

            dont_copy_me = false;
        }
    }


}

void xgm::rectangle::destroy()
{
    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x; j++)
        {
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, white) << 8);
            xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = false;
        }
    }
    
    for(int i = 0; i < this->size_y; i++)
        free(BlankCells[i]);

    free(BlankCells);
    is_destroyed = true;
}

void xgm::rectangle::rotate_right_90()
{
    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x; j++)
        {
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color(black, black) << 8);
            xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = false;
        }
    }

    uint32_t size_x_tmp = this->size_x;
    this->size_x = this->size_y;
    this->size_y = size_x_tmp;

    // bool** blank_cells_tmp = (bool**)calloc(sizeof(bool*) * this->size_y);

    // for(int i = 0; i < this->size_y; i++)
    //     blank_cells_tmp[i] = (bool*)calloc(sizeof(bool) * this->size_x);

    // for(int i = 0; i < this->size_y; i++)
    // {
    //     for(int j = 0; j < this->size_x; j++)
    //         blank_cells_tmp[i][j] = BlankCells[i][j];
    // }

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x; j++)
        {
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (set_output_color((this->color), black) << 8);
            xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = true;
        }
    }
}

xgm::rectangle::rectangle(uint32_t class_id=0)
{
    this->class_id = class_id;
    this->BlankCells = nullptr;
}

std::pair<uint32_t, uint32_t> xgm::rectangle::size_get()
{
    return std::pair<uint32_t, uint32_t> (this->size_x, this->size_y);
}

std::pair<uint32_t, uint32_t> xgm::rectangle::position_get()
{
    return std::pair<uint32_t, uint32_t> (this->position_x, this->position_y);
}

uint32_t xgm::rectangle::sizex_get() const
{
    return this->size_x;
}

uint32_t xgm::rectangle::sizey_get() const
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

void xgm::rectangle::cell_remove(uint8_t x, uint8_t y)
{
    Renderer::ScreenManager XgmScreen;

    if(x >= this->size_x || y >= this->size_y)
        return;
    
    BlankCells[y][x] = true;

    XgmScreen[(this->position_y + y) * VGA_WIDTH + this->position_x + x] = xgm::color::black;

}

xgm::CollisionInfo xgm::rectangle::collision_detect()
{
    Renderer::ScreenManager XgmScreen;
    
    for(int i = 0; i < this->size_x; i++)
    {
        if(!this->BlankCells[0][i])
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y - 1][this->position_x + i])
            return xgm::make_collision_info(true, XgmScreen[((this->position_y - 1) * VGA_WIDTH) + this->position_x + i], xgm::Direction::UP);
    }

    for(int i = 0; i < this->size_x; i++)
    {
        if(!this->BlankCells[this->size_y - 1][i])
        {
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y + this->size_y][this->position_x + i])
                return xgm::make_collision_info(true, XgmScreen[(this->position_y + this->size_y) * VGA_WIDTH + this->position_x + i], xgm::Direction::DOWN); 
        }
    }

    for(int i = 0; i < this->size_y; i++)
    {

        if(!this->BlankCells[i][0])
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x - 1])
                return xgm::make_collision_info(true, XgmScreen[(this->position_x - 1) + (this->position_y + i) + i], xgm::Direction::LEFT);
    }

    for(int i = 0; i < this->size_y; i++)
    {
        if(!this->BlankCells[i][this->size_x - 1])
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + this->size_x])
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

void xgm::Renderer::ScreenManager::screen_clear(void)
{
    
    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
    {
        uint8_t* vram = (uint8_t*)VGA_TEXT_MEMORY;
        vram[i] = (uint8_t)NULL;
    }

    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        for(int j = 0; j < VGA_WIDTH; j++)
            this->screen_cells[i];
    }
}

void xgm::Renderer::ScreenManager::vertical_line_create(uint8_t x, xgm::color::ColorAttributes color)
{       
    for(int i = 0; i < VGA_HEIGHT; i++)   
    {
        (*this)[x + i * VGA_WIDTH] = static_cast<uint8_t> (color);
        this->screen_cells[i][x] = true;
    }
}



extern "C" void __cxa_pure_virtual()
{
    printk("virtual function execution error");
    return;
}