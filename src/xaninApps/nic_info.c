
#include <devices/PCI/pci.h>
#include <libc/stdiox.h>
#include <netapi/network_device.h>

extern pci_device* i8254x_pci_info_get(void);
extern uint8_t* i8254x_mac_get(void);
extern uint32_t i8254x_receive_descriptors_buffer_get(void);
extern uint32_t i8254x_transmit_descriptors_buffer_get(void);
extern uint32_t i8254x_transmit_buffer_get(void);
extern uint32_t i8254x_receive_buffer_get(void);

int nic_info(char* network_device_name)
{

    pci_device* nic_info = i8254x_pci_info_get();

    
    NetworkHandler* device_info = device_info_get_by_name(network_device_name);
    
    if(device_info == NULL)
    {
        xprintf("%zNo Such Device: %s\n", stderr, network_device_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    xprintf("0x%x\n", device_info);
    // xprintf("Network card name: %s\n", nic_info->device_name);

    xprintf("vendor id: 0x%X\n", device_info->pci_info->vendor_id);
    xprintf("device id: 0x%X\n", device_info->pci_info->device_id);
    
    xprintf("base: 0x%X\n", device_info->pci_info->base0);
    xprintf("base1: 0x%X\n", device_info->pci_info->base1);
    xprintf("base2: 0x%X\n", device_info->pci_info->base2);
    xprintf("base3: 0x%X\n", device_info->pci_info->base3);
    xprintf("base4: 0x%X\n", device_info->pci_info->base4);
    xprintf("base5: 0x%X\n", device_info->pci_info->base5);

    xprintf("subsystem id: 0x%x\n", device_info->pci_info->subsystem_id);

    xprintf("MAC address: ");

    uint8_t* mac = device_info->device_mac;
    // uint8_t* mac = i8254x_mac_get();
    for(int i = 0; i < 5; i++)
        xprintf("%x:", mac[i]);
    xprintf("%x\n", mac[5]);

    xprintf("interrupt line (PIC): 0x%x\n", device_info->pci_info->interrupt_line);


    while(getscan() != ENTER);
    return XANIN_OK;


}

