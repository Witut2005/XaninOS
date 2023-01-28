
#include <game_engine/xagame.hpp>
#include <libcpp/ostream.h>
#include <libcpp/istream.h>
#include <libcpp/cmemory.h>
#include <keyboard/scan_codes.h>
#include <libc/stdlibx.h>
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

    
    uint8_t* vram_copy = (uint8_t*)calloc(VGA_SCREEN_RESOLUTION);
    memcpy(vram_copy, (uint8_t*)VGA_TEXT_MEMORY, VGA_SCREEN_RESOLUTION);

    xgm::Renderer::ScreenManager TetrisScreen;
    for(int i = 0; i < VGA_WIDTH; i++)
        TetrisScreen[row * VGA_WIDTH + i] = xgm::color::black;

    // screen_clear();
    
    if(!row)
    {
        free(vram_copy);
        return;
    }

    memcpy((uint8_t*)VGA_TEXT_MEMORY + (VGA_WIDTH * 2), (uint8_t*)vram_copy, row * VGA_WIDTH * 2);

    free(vram_copy);

}

extern "C" int tetris(void)
{

    bool screen_cells[28][22] = {0};

    xgm::Renderer::ScreenManager TetrisScreen;

    screen_clear();
    uint8_t current_color = 5;

    TetrisScreen.vertical_line_create(19 + 5, xgm::color::lgreen);
    TetrisScreen.vertical_line_create(59 - 5, xgm::color::lgreen);

    while(1)
    {

        TetrisScreen.vertical_line_create(19 + 5, xgm::color::lgreen);
        TetrisScreen.vertical_line_create(59 - 5, xgm::color::lgreen);

        if(current_color == 15)
            current_color = 2;

        xgm::rectangle object = xgm::rectangle(0); 

        switch(rand() % 5)
        {
            case 0: object.create(40, 1, 2, 2, current_color); break;
            case 1: object.create(40, 1, 1, 4, current_color); break;
            case 2: {
                object.create(40, 1, 3, 2, current_color); 
                object.cell_remove(0,0);
                object.cell_remove(2,0);
                break;
            }

            case 3:{
                object.create(40, 1, 2, 3, current_color);
                object.cell_remove(1, 0);
                object.cell_remove(1, 1);
                break;
            }

            case 4:{
                object.create(40, 1, 3, 2, current_color);
                object.cell_remove(0,0);
                object.cell_remove(2, 1);
                break;
            }


        }

        while(object.positiony_get() + object.sizey_get() < VGA_HEIGHT)
        {
            xgm::CollisionInfo CollisionStatus = object.collision_detect();

            Screen.x = 0;
            Screen.y = 5;

            // if(CollisionStatus.side != xgm::Direction::NONE)
            // {
            //     xprintf("colision status: %d\n", CollisionStatus.side);
            //     xprintf("color : %d\n", CollisionStatus.color >> 4);
            // }

            if(CollisionStatus.side == xgm::Direction::DOWN)
                break;

            if(std::KeyInfo.scan_code == ARROW_LEFT)
            {
                if(CollisionStatus.side != xgm::Direction::LEFT)
                    object.move(-1, 0);
            }
            else if(std::KeyInfo.scan_code == ARROW_RIGHT)
            {
                if(CollisionStatus.side != xgm::Direction::RIGHT)
                    object.move(1,0);
            }
            
            CollisionStatus = object.collision_detect();

            if(CollisionStatus.side != xgm::Direction::DOWN)
                object.move(0,1);

            msleep(150);
        
        }

        for(int i = 0; i < VGA_HEIGHT; i++)
        {
            bool is_line_full = true;        

            for(int j = 25; j < 53; j++)
            {
                if(TetrisScreen[i * VGA_WIDTH + j] == xgm::color::black)
                {
                    is_line_full = false;
                    break;
                }   
            }

            if(is_line_full)
                row_down(i);


        }

        // for(int i = 0; i < object.sizey_get(); i++)
        // {
        //     for(int j = 0; i < object.sizex_get(); i++)
        //         screen_cells[object.positiony_get()][object.positionx_get() + j] = true;
        // }

        current_color++;
    }

}
