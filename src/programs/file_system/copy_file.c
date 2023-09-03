
#include <fs/xin_syscalls.h>

__STATUS xin_copy_app(char* file_name, char* new_file_name)
{
    int status = __sys_xin_copy(file_name, new_file_name);

    if(status == XANIN_OK)
        return status;
    else if(status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zFILE NOT FOUND: %s\n", stderr, file_name);

    return status;
}