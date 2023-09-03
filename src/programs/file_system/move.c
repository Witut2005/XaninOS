


#include <fs/xin.h>

int xin_move_entry_app(char* entry_name, char* new_name)
{
    int status = __sys_xin_entry_move(entry_name, new_name);

    if(status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zNO SUCH ENTRY: %s\n", stderr, entry_name);
        
    return status;
}