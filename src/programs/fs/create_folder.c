

#include <fs/xin.h>
#include <lib/libc/stdiox.h>

// TERMINAL_APP

int xin_create_folder_app(char* entry_name)
{
    int status = __xin_folder_create(entry_name);

    if (status == XANIN_OK)
        return status;
    else if (status == XIN_ENTRY_EXISTS)
        xprintf("%zFOLDER ALREADY EXISTS: %s\n", OUTPUT_COLOR_ERROR_SET, entry_name);
    else if (status == XANIN_ERROR)
        xprintf("%zFOLDER CREATION ERROR: %s\n", OUTPUT_COLOR_ERROR_SET, entry_name);
    else if (status == XIN_BAD_FOLDER_NAME)
        xprintf("%zBAD FOLDER NAME MISSING / CHARACTER AT THE END\n", OUTPUT_COLOR_ERROR_SET);

    return status;
}