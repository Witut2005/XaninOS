
#include <stdint.h>
#include <game_engine/xagame.hpp>
#include <libc/colors.h>

extern "C" void screen_clear(void);
extern "C" void msleep(float miliseconds);
extern "C" void* malloc(uint16_t size);

using namespace xgm;

extern "C" void xagame_test(void)
{

    screen_clear();


    rectangle* y = new rectangle(0);
    y->create(20,5,10,5, yellow);

    ScreenManager manager;
    manager[2] = xgm::color::red | xgm::color::green_text;


    while(1)
    {
        msleep(1000);
        y->move(0,1);
    }

}