#include <lib/libc/canvas.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <sys/input/input.h>

static void timer_handler(KeyInfo ki, void* args)
{
    if (ki.scan_code == ENTER)
        exit();
}

// CANVAS_APP
int timer_test(void)
{

    stdio_mode_set(STDIO_MODE_CANVAS);

    uint32_t current_time = 0;

    canvas_screen_clear();
    KeyInfo k;

    __sys_input_add_object_to_observe(input_observable_create(k, input_observed_object_options_create(true)));

    InputHandler tmp = input_handler_create(timer_handler, input_handler_options_create(NULL, INPUT_USER));
    __sys_input_add_handler(&tmp);

    canvas_xprintf("nicho: 0x%x\n", timer_handler);
    canvas_xprintf("Press 'a' to start...");

    while (getchar() != 'a')
        ;

    while (1)
    {
        canvas_screen_clear();
        canvas_xprintf("%d", current_time);
        current_time++;
        msleep(1000);
    }

    __sys_input_remove_object_from_observe(&k);

    return XANIN_OK;
}
