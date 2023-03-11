
#include <stdint.h>
#include <game_engine/xagame.hpp>
#include <libc/colors.h>
#include <libcpp/new.hpp>

extern "C" int screen_clear(void);
extern "C" void msleep(float miliseconds);

using namespace xgm;
using namespace xgm::Renderer;

extern "C" int xagame_test(void)
{

    screen_clear();

    rectangle* y = new rectangle(4);

    GeometryObject* ptr = y;
    ptr->create(20,5,10,5, yellow);

    // ScreenManager manager;
    // manager[2] = xgm::color::red | xgm::color::green_text;
    // manager.vertical_line_create(5, xgm::color::yellow);


    // while(1)
    {
        msleep(1000);
        ptr->move(0,1);
    }
    return 0;

}