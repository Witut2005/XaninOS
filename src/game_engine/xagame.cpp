
#include <game_engine/xagame.hpp>
#include <libc/colors.h>
#include <libc/stdlibx.h>
#include <libcpp/algorithm.h>
#include <libc/hal.h>


extern "C" void printk(char* str);

bool xgm::Renderer::ScreenManager::screen_cells[VGA_HEIGHT][VGA_WIDTH] = {false};

// xgm::CollisionInfo xgm::make_collision_info(bool x, uint8_t y, xgm::Direction z)
// {
//     xgm::CollisionInfo instance = {x,y, z == xgm::Direction::RIGHT, z == xgm::Direction::LEFT, 
//                                     z == xgm::Direction::UP, z == xgm::Direction::DOWN};
//     return instance;
// }

void xgm::rectangle::create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color)
{        
    this->is_destroyed = false;

    this->position_x = x;
    this->position_y = y;
    this->size_x = size_x;
    this->size_y = size_y;
    this->color = color;

    if(BlankCells != NULL)
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
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (OUTPUT_COLOR_SET(color, white) << 8);
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
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (OUTPUT_COLOR_SET(black, black) << 8);
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
                Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (OUTPUT_COLOR_SET((this->color), white) << 8);
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
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (OUTPUT_COLOR_SET(black, white) << 8);
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
            Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (OUTPUT_COLOR_SET(black, black) << 8);
            xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = false;
        }
    }

    uint32_t size_x_tmp = this->size_x;
    this->size_x = this->size_y;
    this->size_y = size_x_tmp;
    
    {
        auto deallocate = BlankCells;
        auto tmp = std::matrix90_rotate_right<bool>(BlankCells, this->size_y, this->size_x);
        BlankCells = tmp;


        for(int i = 0; i < this->size_x; i++)
            free(deallocate[i]);

        free(deallocate);

    }

    for(int i = 0; i < this->size_y; i++)
    {
        for(int j = 0; j < this->size_x; j++)
        {
            if(!this->BlankCells[i][j])
            {
                Screen.cursor[this->position_y + i][this->position_x + j] = ' ' | (OUTPUT_COLOR_SET((this->color), black) << 8);
                xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + j] = true;
            }
        }
    }

    for(int i = 0; i < 10000; i++)
        io_wait();
}

xgm::rectangle::rectangle(uint32_t class_id=0) : class_id(class_id), BlankCells(NULL), rotation_count(0)
{
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

uint32_t xgm::rectangle::positionx_get() const
{
    return this->position_x;
}

uint32_t xgm::rectangle::positiony_get() const
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

    xgm::CollisionInfo ColInfo = {false, false, false, false};
    
    for(int i = 0; i < this->size_x; i++)
    {
        if(!this->BlankCells[0][i])
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y - 1][this->position_x + i])
                ColInfo.up = true;
    }

    for(int i = 0; i < this->size_x; i++)
    {
        if(!this->BlankCells[this->size_y - 1][i])
        {
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y + this->size_y][this->position_x + i])
                ColInfo.down = true;
        }
    }

    for(int i = 0; i < this->size_y; i++)
    {

        if(!this->BlankCells[i][0])
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x - 1])
                ColInfo.left = true;
    }

    for(int i = 0; i < this->size_y; i++)
    {
        if(!this->BlankCells[i][this->size_x - 1])
            if(xgm::Renderer::ScreenManager::screen_cells[this->position_y + i][this->position_x + this->size_x])
                ColInfo.right = true;
    }

    return ColInfo;

}

uint8_t xgm::rectangle::color_get() const
{
    return this->color;
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
            this->screen_cells[i][j] = false;
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

xgm::ColissionDetector::ColissionDetector(uint8_t x, uint8_t y, uint8_t size_x, uint8_t size_y) : x(x), y(y), size_x(size_x), size_y(size_y){
    // xgm::Renderer::ScreenManager ScreenTmp;


    // Screen.x = 0;
    // Screen.y = 0;
    // xprintf("x: %d\n", x);
    // xprintf("y: %d\n", y);
    // xprintf("size_x: %d\n", size_x);
    // xprintf("size_y: %d\n", size_y);
    
    // for(int i = 0; i < size_y; i++)
    //     for(int j = 0; j < size_x; j++)
    //         ScreenTmp[i * VGA_WIDTH + j] = xgm::color::white;
}

std::pair<uint32_t, uint32_t> xgm::ColissionDetector::check(xgm::rectangle ObjectToIgnore)
{
    
    for(int i = 1; i < this->size_y; i++)
    {
        for(int j = 1; j < this->size_x; j++)
        {
            if(xgm::Renderer::ScreenManager::screen_cells[this->y + i][this->x + j])
            {
                if(!((ObjectToIgnore.positionx_get() + j == this->x + j) && (ObjectToIgnore.positiony_get() + i == this->y + i)))
                    return std::pair<uint32_t, uint32_t> (this->x + j, this->y + i);
            }
        }
    }
    return std::pair<uint32_t, uint32_t>(0, 0);
}

extern "C" void __cxa_pure_virtual()
{
    printk("virtual function execution error");
    eoi_send();
    interrupt_enable();
    exit();
    return;
}
