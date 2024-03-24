
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_folder_remove_app(char* folder_name)
{
    int status = __xin_folder_remove(folder_name);

    if (status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zNO SUCH FOLDER: %s\n", OUTPUT_COLOR_ERROR_SET, folder_name);
    return XANIN_OK;
}