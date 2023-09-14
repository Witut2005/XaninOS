
#include <fs/xin.h>

//TERMINAL_APP

int xin_list_files_app(char** argv)
{
    int status = __sys_xin_list_files(argv);
    return status;
}