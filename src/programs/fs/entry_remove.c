
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_entry_remove_app(char* entry_name)
{
    __STATUS status = __xin_file_remove(entry_name);

    if (status == XANIN_OK)
    {
    }

    else if (status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zNO SUCH FOLDER: %s\n", OUTPUT_COLOR_ERROR_SET, entry_name);

    else
        xprintf("%zERROR\n", OUTPUT_COLOR_ERROR_SET);

    return status;
}