
#include <game_engine/xagame.hpp>
#include <libcpp/ostream.h>
#include <libcpp/istream.h>
#include <libcpp/cmemory.h>
#include <keyboard/scan_codes.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 28



bool if_row_achieved(int row)
{
    xgm::Renderer::ScreenManager TetrisScreen;
    for(int i = 0; i < VGA_WIDTH; i++)
    {
        if(TetrisScreen[row * VGA_WIDTH + i] == xgm::color::black)
            return false; 
    }

    return true;
}

void row_down(int row)
{

    xgm::Renderer::ScreenManager TetrisScreen;
    for(int i = 0; i < VGA_WIDTH; i++)
        TetrisScreen[row * VGA_WIDTH + i] = xgm::color::black;
    
    memcpy((uint8_t*)(VGA_TEXT_MEMORY + (VGA_WIDTH * 2)), (uint8_t*)VGA_TEXT_MEMORY, VGA_WIDTH * 2);
    memset((uint8_t*)(VGA_TEXT_MEMORY), 0x0, VGA_WIDTH * 2);

}

extern "C" int tetris(void)
{
    xgm::Renderer::ScreenManager TetrisScreen;

    screen_clear();
    uint8_t current_color = 5;

    TetrisScreen.vertical_line_create(19 + 5, xgm::color::lgreen);
    TetrisScreen.vertical_line_create(59 - 5, xgm::color::lgreen);

    while(1)
    {
        if(current_color == 15)
            current_color = 2;

        xgm::rectangle object = xgm::rectangle(0); 
        object.create(20, 5, 2, 3, current_color);

        while(object.positiony_get() + object.sizey_get() < VGA_HEIGHT)
        {
            xgm::CollisionInfo CollisionStatus = object.collision_detect();

            if(CollisionStatus.side == xgm::Direction::DOWN)
                break;

            msleep(100);
            if(std::KeyInfo.scan_code == ARROW_LEFT)
            {
                if(CollisionStatus.side != xgm::Direction::LEFT)
                    object.move(-1, 1);
                else
                    object.move(0,1);
            }
            else if(std::KeyInfo.scan_code == ARROW_RIGHT)
            {
                if(CollisionStatus.side != xgm::Direction::RIGHT)
                    object.move(1,1);
                else
                    object.move(0,1);
            }
            
            else 
                object.move(0,1);
        
        }
        current_color++;
    }

}