
#include <game_engine/xagame.hpp>
#include <libcpp/ostream.h>
#include <libcpp/istream.h>
#include <libcpp/cmemory.h>
#include <keyboard/scan_codes.h>
#include <libc/stdlibx.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 28

int8_t first_nonempty_row_get(void)
{

    xgm::Renderer::ScreenManager TetrisScreen;

    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        for(int j = 0; j < VGA_WIDTH; j++)
        {
            if(TetrisScreen[i * VGA_WIDTH + j] != xgm::color::black)
                return i;
        }
    }
    return -1;
}

void row_down(int row)
{

    
    uint8_t* vram_copy = (uint8_t*)calloc(VGA_SCREEN_RESOLUTION);
    memcpy(vram_copy, (uint8_t*)VGA_TEXT_MEMORY, VGA_SCREEN_RESOLUTION);

    xgm::Renderer::ScreenManager TetrisScreen;

    for(int i = 0; i < VGA_WIDTH; i++)
        TetrisScreen[row * VGA_WIDTH + i] = xgm::color::black;

    
    if(!row)
    {
        memset((uint8_t*)VGA_TEXT_MEMORY, 0, VGA_WIDTH * 2);
        free(vram_copy);
        return;
    }

    memcpy((uint8_t*)VGA_TEXT_MEMORY + (VGA_WIDTH * 2), (uint8_t*)vram_copy, row * VGA_WIDTH * 2);

    for(int i = 0; i < VGA_HEIGHT; i++)
    {
        for(int j = 0; j < VGA_WIDTH; j++)
        {
            if(TetrisScreen[i * VGA_WIDTH + j] != xgm::color::black)
                TetrisScreen.screen_cells[i][j] = true;
            else
                TetrisScreen.screen_cells[i][j] = false;
        }
    }

    free(vram_copy);

}

static xgm::rectangle object(0);

extern "C" void tetris_keyboard_handler(void)
{
    
    xgm::CollisionInfo CollisionStatus;

    CollisionStatus = object.collision_detect();

    if(CollisionStatus.down)
        return;

    if(std::KeyInfo.scan_code == SPACE)
    {

        // xgm::ColissionDetector tmp(object.positionx_get(), object.positiony_get(), object.sizey_get(), object.sizex_get());

        // if(!tmp.check(object.color_get()))
            object.rotate_right_90();
    
    }

    else if(std::KeyInfo.scan_code == ARROW_LEFT)
    {
        if(!CollisionStatus.left)
            object.move(-1, 0);
    }

    else if(std::KeyInfo.scan_code == ARROW_RIGHT)
    {
        if(!CollisionStatus.right)
            object.move(1,0);
    }
    
    else if(std::KeyInfo.scan_code == S_KEY)
        screen_clear();

    else if(std::KeyInfo.scan_code == ARROW_DOWN)
    {
        while(!object.collision_detect().down && ((object.positiony_get() + object.sizey_get()) < VGA_HEIGHT))
            object.move(0, 1);
    }


}


extern "C" int tetris(void)
{

    xgm::Renderer::ScreenManager TetrisScreen;

    TetrisScreen.screen_clear();
    uint8_t current_color = 5;
    uint32_t score = 0;
    
    KEYBOARD_KEYSTROKE_HANLDER_LOAD(tetris_keyboard_handler);

    while(1)
    {


        if(current_color == 15)
            current_color = 2;


        if(TetrisScreen.screen_cells[0][40])
            break;

        switch(rand() % 5)
        {
            case 0: object.create(40, 0, 2, 2, current_color); break;
            case 1: object.create(40, 0, 1, 4, current_color); break;
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

            Screen.x = 0;
            Screen.y = 5;

            xprintf("position x: %d\n", object.positionx_get());
            xprintf("position y: %d\n", object.positiony_get());

            TetrisScreen.vertical_line_create(19 + 10, xgm::color::lgreen);
            TetrisScreen.vertical_line_create(59 - 10, xgm::color::lgreen);

            for(int i = 1; i < 3; i++)
                TetrisScreen.vertical_line_create(19 + 10 - i, xgm::color::black);

            for(int i = 1; i < 3; i++)
                TetrisScreen.vertical_line_create(59 - 10 + i, xgm::color::black);

            // if(!object.collision_detect().down) //&& ((object.positiony_get() + object.sizey_get()) < VGA_HEIGHT))
            //     object.move(0,1);

            // else
            //     break;

            msleep(200);
        }

        for(int i = 0; i < VGA_HEIGHT; i++)
        {
            bool is_line_full = true;        

            for(int j = 30; j < 48; j++)
            {
                if(TetrisScreen[i * VGA_WIDTH + j] == xgm::color::black)
                {
                    is_line_full = false;
                    break;
                }   
            }

            if(is_line_full)
            {
                row_down(i);
                score++;
            }

        }


        current_color++;
    }

    screen_clear();

    Screen.y = 14;
    Screen.x = 35;
    KEYBOARD_KEYSTROKE_HANLDER_UNLOAD();
    xprintf("Your score: %d\n", score);
    while(inputg().scan_code != ENTER);

}
