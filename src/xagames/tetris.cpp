
#include <game_engine/xagame.hpp>
#include <libcpp/ostream.h>
#include <libcpp/cmemory.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 28


bool if_row_achieved(int row)
{
    xgm::ScreenManager TetrisScreen;
    for(int i = 0; i < VGA_WIDTH; i++)
    {
        if(TetrisScreen[row * VGA_WIDTH + i] == xgm::color::black)
            return false; 
    }

    return true;
}

void row_down(int row)
{
    xgm::ScreenManager TetrisScreen;

    for(int i = 0; i < VGA_WIDTH; i++)
        TetrisScreen[row * VGA_WIDTH + i] = xgm::color::black;
    
    memcpy((uint8_t*)(VGA_TEXT_MEMORY + (VGA_WIDTH * 2)), (uint8_t*)VGA_TEXT_MEMORY, VGA_WIDTH * 2);
    memset((uint8_t*)(VGA_TEXT_MEMORY), 0x0, VGA_WIDTH * 2);

}

extern "C" int tetris(void)
{
    xgm::ScreenManager TetrisScreen;
    
    for(int j = 0; j < VGA_WIDTH; j++)
        TetrisScreen[j] = xgm::color::red;

    // for(int i = 0; i < VGA_HEIGHT - 1; i++)
    // {
    //     for(int j = 0; j < VGA_WIDTH; j++)
    //         TetrisScreen[i * VGA_WIDTH + j] = xgm::color::red;
    // }

    {
        // std::cout << if_row_achieved(27) << std::endl;
        row_down(1);
    }
    while(1);

}