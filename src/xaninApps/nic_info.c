
#include <devices/PCI/pci.h>
#include <libc/stdiox.h>

extern pci_device* i8254x_pci_info_get(void);
extern uint8_t* i8254x_mac_get(void);
extern uint32_t i8254x_receive_buffer_get(void);
extern uint32_t i8254x_transmit_buffer_get(void);

void nic_info(void)
{

    pci_device* nic_info = i8254x_pci_info_get();

    xprintf("bus: 0x%X\n", nic_info->bus);
    xprintf("slot: 0x%X\n", nic_info->slot);
    xprintf("function: 0x%X\n", nic_info->function);
    xprintf("offset: 0x%X\n", nic_info->offset);
    xprintf("vendor id: 0x%X\n", nic_info->vendor_id);
    xprintf("device id: 0x%X\n", nic_info->device_id);
    
    xprintf("base: 0x%X\n", nic_info->base0);
    xprintf("base1: 0x%X\n", nic_info->base1);
    xprintf("base2: 0x%X\n", nic_info->base2);
    xprintf("base3: 0x%X\n", nic_info->base3);
    xprintf("base4: 0x%X\n", nic_info->base4);
    xprintf("base5: 0x%X\n", nic_info->base5);

    xprintf("subsystem id: 0x%x\n", nic_info->subsystem_id);

    xprintf("MAC address: ");

    uint8_t* mac = i8254x_mac_get();
    for(int i = 0; i < 5; i++)
        xprintf("%x:", mac[i]);
    xprintf("%x\n", mac[5]);

    xprintf("interrupt line (PIC): 0x%x\n", nic_info->interrupt_line);

    xprintf("receive buffer address: 0x%x\n", i8254x_receive_buffer_get());
    xprintf("transmit buffer address: 0x%x\n", i8254x_transmit_buffer_get());

    while(getscan() != ENTER);



}

