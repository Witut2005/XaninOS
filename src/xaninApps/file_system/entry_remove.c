
#include <xin_fs/xin.h>

int XinEntry_remove_app(char* entry_name)
{
    int status = __sys_XinEntry_remove(entry_name);

    if(status == XIN_ENTRY_NOT_FOUND) 
    {
        xprintf("%zNO SUCH FOLDER: %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
    }

    return status;

    
}