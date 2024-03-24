
#include <fs/xin.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_move_entry_app(char* entry_name, char* new_name)
{
    int status = __xin_entry_move(entry_name, new_name);

    if (status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zNO SUCH ENTRY: %s\n", OUTPUT_COLOR_ERROR_SET, entry_name);

    return status;
}