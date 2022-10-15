
#include <netapi/network_device.h>

int network_device_rename(char* oldname, char* newname)
{

    NetworkHandler* handler = device_info_get_by_name(oldname);

    if(handler == nullptr)
    {
        xprintf("%zNo Such Device %s\n", stderr, oldname);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else
    {
        memset(handler->device_name, 0, 50);

        if(strlen(newname) >= 50)
        {
            xprintf("%Max Device Name Length Is 49 %s\n", stderr, oldname);
            while(KeyInfo.scan_code != ENTER);
            return XANIN_ERROR;
        }

        strcpy(handler->device_name, newname);
    }

    return XANIN_OK;

}