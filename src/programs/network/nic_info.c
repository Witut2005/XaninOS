
#include <sys/devices/pci/pci.h>
#include <lib/libc/stdiox.h>
#include <sys/net/netapi/network_device.h>
#include <sys/input/input.h>

//TERMINAL_APP

int nic_info(const char* network_device_name)
{

    pci_device* NetDevice = netapi_device_info_get(network_device_name);

    if (NetDevice == NULL)
    {
        xprintf("%zNo Such Device: %s\n", OUTPUT_COLOR_ERROR_SET, network_device_name);
        return XANIN_ERROR;
    }

    xprintf("0x%x\n", NetDevice);
    // xprintf("Network card name: %s\n", nic_info->device_name);

    xprintf("vendor id: 0x%X\n", NetDevice->vendor_id);
    xprintf("device id: 0x%X\n", NetDevice->device_id);

    xprintf("base: 0x%X\n", NetDevice->base0);
    xprintf("base1: 0x%X\n", NetDevice->base1);
    xprintf("base2: 0x%X\n", NetDevice->base2);
    xprintf("base3: 0x%X\n", NetDevice->base3);
    xprintf("base4: 0x%X\n", NetDevice->base4);
    xprintf("base5: 0x%X\n", NetDevice->base5);

    xprintf("subsystem id: 0x%x\n", NetDevice->subsystem_id);

    xprintf("MAC address: ");

    uint8_t* mac = netapi_device_mac_get(network_device_name);
    // uint8_t* mac = i8254x_mac_get();
    for (int i = 0; i < 5; i++)
        xprintf("%x:", mac[i]);
    xprintf("%x\n", mac[5]);

    xprintf("interrupt line (PIC): 0x%x\n", NetDevice->interrupt_line);

    return XANIN_OK;
}

