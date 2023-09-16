
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

__STATUS pwd(void)
{
    xprintf("%s\n", xin_current_directory); // USE FUNCTION HERE
    return XANIN_OK;
}