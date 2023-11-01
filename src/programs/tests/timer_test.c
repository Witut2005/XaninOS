#include <lib/libc/time.h>
#include <lib/libc/canvas.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <sys/input/input.h>

static bool timer_handler(key_info_t ki, uint8_t **a)
{
    if ((ki.scan_code == ENTER) && (!app_exited))
        exit();
    return true;
}

// CANVAS_APP
int timer_test(void)
{

    stdio_mode_set(STDIO_MODE_CANVAS);

    uint32_t current_time = 0;

    canvas_screen_clear();
    key_info_t k;

    KeyboardModuleObservedObjectOptions Options = {true};
    __input_module_add_object_to_observe(&k, Options);
    __input_module_add_handler(input_module_handler_create(timer_handler, NULL));

    canvas_xprintf("Press 'a' to start...");
    while (getchar() != 'a')
        ;

    while (1)
    {
        canvas_screen_clear();
        canvas_xprintf("%d", current_time);
        canvas_xprintf(" 0x%x", k.scan_code);
        current_time++;
        msleep(1000);
    }

    __input_module_remove_object_from_observe(&k);

    return XANIN_OK;
}