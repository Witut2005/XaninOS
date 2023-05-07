
#include <fs/xin.h>

int xin_entry_remove_app(char* entry_name)
{
    int status = __sys_xin_entry_remove(entry_name);

    if(status == XIN_ENTRY_NOT_FOUND) 
    {
        xprintf("%zNO SUCH FOLDER: %s\n", stderr, entry_name);
        while(inputg().scan_code != ENTER);
    }

    return status;

    
}