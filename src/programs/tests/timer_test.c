#include <lib/libc/time.h>
#include <lib/libc/canvas.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <sys/input/input.h>

//CANVAS_APP

void timer_input(void)
{
    if(getscan() == ENTER)
        app_exited = true;
}

int timer_test(void)
{

    stdio_mode_set(STDIO_MODE_CANVAS);
    keyboard_handle = timer_input;

    uint32_t current_time = 0;

    canvas_screen_clear();

    canvas_xprintf("Press 'a' to start...");
    while(inputc() != 'a');

    while(!app_exited)
    {
        canvas_screen_clear();
        canvas_xprintf("%d", current_time);
        current_time++;
        msleep(1000);
    }

    return XANIN_OK;

}