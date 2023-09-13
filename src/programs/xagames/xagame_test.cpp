
#include <stdint.h>
#include <lib/game_engine/xagame.hpp>
#include <lib/libc/colors.h>
#include <lib/libcpp/new.hpp>
#include <lib/libc/canvas.h>

//CANVAS_APP

extern "C" int screen_clear(void);
extern "C" void msleep(float miliseconds);

using namespace xgm;
using namespace xgm::Renderer;

extern "C" int xagame_test(void)
{

    stdio_mode_set(STDIO_MODE_CANVAS);
    canvas_screen_clear();

    rectangle* y = new rectangle(4);

    GeometryObject* ptr = y;
    ptr->create(20,5,10,5, yellow);

    // ScreenManager manager;
    // manager[2] = xgm::color::red | xgm::color::green_text;
    // manager.vertical_line_create(5, xgm::color::yellow);

    key_info_t KeyInfo;
    __sys_keyinfo_get(&KeyInfo);

    while(KeyInfo.scan_code != ENTER)
    {
        __sys_keyinfo_get(&KeyInfo);
        msleep(1000);
        ptr->move(0,1);
    }
    return 0;

}