
#include <fs/xin.h>
#include <fs/xin_syscalls.h>
#include <lib/libc/stdiox.h>

//TERMINAL_APP

int xin_entry_remove_app(char* entry_name)
{
    int status = __sys_xin_entry_remove(entry_name);

    if(status == XIN_ENTRY_NOT_FOUND) 
        xprintf("%zNO SUCH FOLDER: %s\n", stderr, entry_name);

    return status;
}