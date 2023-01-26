

#include <xin_fs/xin.h>

int xin_create_folder_app(char* entry_name)
{
    int status = __sys_xin_folder_create(entry_name);

    if(status == XANIN_OK)
        return status;
    else if(status == XIN_FILE_EXISTS)
        xprintf("%zFILE ALREADY EXISTS: %s\n", stderr, entry_name);
    else if(status == XANIN_ERROR)
        xprintf("%zFILE CREATION ERROR: %s\n", stderr, entry_name);
    else if(status == XIN_BAD_FOLDER_NAME)
        xprintf("%zBAD FOLDER NAME MISSING / CHARACTER AT THE END\n", stderr);

    while(inputg().scan_code != ENTER);

    return status;

}