
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <sys/net/netapi/network_device.h>

//TERMINAL_APP

int nic_print(void)
{
    PciDevicePack* NetworkDevices = (PciDevicePack*)calloc(SIZE_OF(PciDevicePack));

    netapi_all_cards_info_get(NetworkDevices);

    xprintf("Number of Devices: %d\n\n", NetworkDevices->length);

    for(int i = 0; i < NetworkDevices->length; i++)
    {
        xprintf("%s\n", NetworkDevices->name);
        xprintf("ven: %X dev: %X\n", NetworkDevices->PciInfo[i]->vendor_id, NetworkDevices->PciInfo[i]->device_id);
        xprintf("%z----------------------------------\n", OUTPUT_COLOR_SET(black, green));
    }

    free(NetworkDevices);

    return XANIN_OK;
}
