
#include <stdint.h>
#include <game_engine/xagame.cpp>

extern "C" void screen_clear(void);
extern "C" void msleep(float miliseconds);
extern "C" void* malloc(uint16_t size);

extern "C" void xagame_test(void)
{

    screen_clear();


    xgm::rectangle y(0);
    y.create(20,5,10,5, yellow);

    while(1)
    {
        msleep(1000);
        y.rotate_right_90();
    }

}