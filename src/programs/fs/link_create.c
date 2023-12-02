
#include <fs/xin.h>
#include <fs/xin_syscalls.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

__STATUS xin_link_create_app(char *file_name, char *link_name)
{
    int status = __xin_link_create(file_name, link_name);

    if (status == XANIN_OK)
        return status;

    else if (status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zFILE NOT FOUND: %s\n", stderr, file_name);

    return status;
}