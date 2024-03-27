
#include <lib/libc/stdiox.h>
#include <lib/libc/hal.h>
#include <sys/devices/keyboard/keyboard.h>

int reboot(void)
{
    keyboard_cpu_reset();
    return XANIN_OK;
}
