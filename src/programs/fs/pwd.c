
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

__STATUS pwd(void)
{
    char buf[XIN_MAX_PATH_LENGTH];
    xprintf("%s\n", __xin_current_directory_get(buf)); // USE FUNCTION HERE
    return XANIN_OK;
}