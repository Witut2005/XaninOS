
#include <fs/xin.h>

int xin_list_files_app(char** argv)
{
    int status = __sys_xin_list_files(argv);
    return status;
}