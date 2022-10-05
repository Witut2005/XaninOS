
#include <netapi/network_device.h>

void nic_print(void)
{

    NetworkHandler* device_info = NetworkHandlers;

    for(int i = 0; i < 10; i++)
    {
        if(device_info[i].is_device_present)
        {
            xprintf("%s\n", device_info[i].device_name);
            xprintf("ven: %X dev: %X\n", device_info[i].pci_info->vendor_id, device_info[i].pci_info->device_id);
            xprintf("%z----------------------------------\n", set_output_color(black, green));
        }
    }

    while(KeyInfo.scan_code != ENTER);

}
