
#include <lib/libc/colors.h>
#include <lib/libc/endian.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/time.h>
#include <lib/screen/screen.h>
#include <lib/system/system.h>
#include <sys/devices/pit/pit.h>

uint8_t null_region[20] = { 0 };
uint8_t* null_memory_region;

char getchar(void)
{
    return __sys_getchar();
}

char getscan(void)
{
    return __sys_getscan();
}

xchar getxchar(void)
{
    return __sys_getxchar();
}

xchar inputg(void)
{
    return __sys_inputg();
}

bool app_exited = false;
