
#include <stdint.h>
#include <game_engine/xagame.hpp>
#include <libc/colors.h>

extern "C" int screen_clear(void);
extern "C" void msleep(float miliseconds);
extern "C" void* malloc(uint16_t size);

using namespace xgm;
using namespace xgm::Renderer;

extern "C" int xagame_test(void)
{

    screen_clear();

    rectangle y(0);// = (rectangle*)malloc(sizeof(rectangle));
    y.create(20,5,10,5, yellow);

    // ScreenManager manager;
    // manager[2] = xgm::color::red | xgm::color::green_text;
    // manager.vertical_line_create(5, xgm::color::yellow);


    // while(1)
    {
        msleep(1000);
        y.move(0,1);
    }
    return 0;

}