
#include <fs/xin.h>
#include <fs/xin_syscalls.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_create_file_app(char *entry_name)
{
    int status = __xin_file_create(entry_name);

    if (status == XANIN_OK)
        return status;
    else if (status == XIN_FILE_EXISTS)
        xprintf("%zFILE ALREADY EXISTS: %s\n", stderr, entry_name);
    else if (status == XANIN_ERROR)
        xprintf("%zFILE CREATION ERROR: %s\n", stderr, entry_name);

    return status;
}