
#include <xin_fs/xin.h>

int xin_folder_remove_app(char* folder_name)
{
    int status = __sys_xin_folder_remove(folder_name);
    if(status == XIN_ENTRY_NOT_FOUND)
    {
        xprintf("%zNO SUCH FOLDER: %s\n", stderr, folder_name);
        while(inputg().scan_code != ENTER);
    }
    return XANIN_OK;
}