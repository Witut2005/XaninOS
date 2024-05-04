
#include <fs/xin.h>
#include <lib/libc/file.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_folder_change_app(const char* new_folder_name)
{
    int status = __xin_folder_change(new_folder_name);

    if (status != XANIN_OK)
        xprintf("%zNO SUCH FOLDER: %s\n", OUTPUT_COLOR_ERROR_SET, new_folder_name);

    return status;
}