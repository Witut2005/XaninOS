
#include <devices/PCI/pci.h>
#include <libc/stdiox.h>
#include <netapi/network_device.h>

extern pci_device* i8254x_pci_info_get(void);
extern uint8_t* i8254x_mac_get(void);
extern uint32_t i8254x_receive_descriptors_buffer_get(void);
extern uint32_t i8254x_transmit_descriptors_buffer_get(void);
extern uint32_t i8254x_transmit_buffer_get(void);
extern uint32_t i8254x_receive_buffer_get(void);

int nic_info(const char* network_device_name)
{

    pci_device* NetDevice = netapi_device_info_get(network_device_name);
    
    if(NetDevice == NULL)
    {
        xprintf("%zNo Such Device: %s\n", stderr, network_device_name);
        while(inputg().scan_code != ENTER);
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
    for(int i = 0; i < 5; i++)
        xprintf("%x:", mac[i]);
    xprintf("%x\n", mac[5]);

    xprintf("interrupt line (PIC): 0x%x\n", NetDevice->interrupt_line);


    while(getscan() != ENTER);
    return XANIN_OK;


}

