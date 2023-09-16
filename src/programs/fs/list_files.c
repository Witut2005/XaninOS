
#include <fs/xin.h>
#include <fs/xin_syscalls.h>
#include <lib/libc/stdiox.h>

//TERMINAL_APP

int xin_list_files_app(char** argv)
{
    int status = __sys_xin_list_files(argv);
    return status;
}