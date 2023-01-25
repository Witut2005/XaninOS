


#include <xin_fs/xin.h>

int xin_move_entry_app(char* entry_name, char* new_name)
{
    int status = __sys_XinEntry_move(entry_name, new_name);

    if(status == XIN_ENTRY_NOT_FOUND)
    {
        xprintf("%zNO SUCH ENTRY: %s\n", stderr, entry_name);
        while(KeyInfo.scan_code != ENTER);
    }
    return status;

}