
#include <fs/xin.h>

int xin_folder_change_app(const char* new_folder_name)
{
    int status = __sys_xin_folder_change(new_folder_name);

    if(status == XIN_ENTRY_NOT_FOUND) 
    {
        xprintf("%zNO SUCH FOLDER: %s\n", stderr, new_folder_name);
        while(inputg().scan_code != ENTER);
    }

    return status;

    
}