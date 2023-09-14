
#include <sys/net/netapi/network_device.h>

//TERMINAL_APP

int nic_rename(char* oldname, char* newname)
{
    pci_device* NetDevice = netapi_device_info_get(oldname);

    if(NetDevice == NULL)
    {
        xprintf("%zNo Such Device %s\n", stderr, oldname);
        return XANIN_ERROR;
    }

    else
    {
        int status = netapi_rename_device(oldname, newname);
        if(status != XANIN_OK)
            xprintf("%zERROR status code: %d\n", stderr, status);
    }

    return XANIN_OK;
}