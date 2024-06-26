
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_create_file_app(char* entry_name)
{
    int status = __xin_file_create(entry_name);

    if (status == XANIN_OK)
        return status;
    else if (status == XIN_ENTRY_EXISTS)
        xprintf("%zFILE ALREADY EXISTS: %s\n", OUTPUT_COLOR_ERROR_SET, entry_name);
    else if (status == XANIN_ERROR)
        xprintf("%zFILE CREATION ERROR: %s\n", OUTPUT_COLOR_ERROR_SET, entry_name);

    return status;
}