
#include <fs/xin.h>
#include <fs/xin_syscalls.h>
#include <lib/libc/stdiox.h>

//TERMINAL_APP

int xin_folder_remove_app(char* folder_name)
{
    int status = __sys_xin_folder_remove(folder_name);
    
    if(status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zNO SUCH FOLDER: %s\n", stderr, folder_name);
    return XANIN_OK;
}