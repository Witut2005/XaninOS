
#include <xin_fs/xin.h>

__STATUS xin_link_create_app(char* file_name, char* link_name)
{
    int status = __sys_xin_link_create(file_name, link_name);

    if(status == XANIN_OK)
        return status;
    
    else if(status == XIN_ENTRY_NOT_FOUND)
        xprintf("%zFILE NOT FOUND: %s\n", stderr, file_name);
    
    while(KeyInfo.scan_code != ENTER);
    return status;

}