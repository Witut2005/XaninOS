
#include <devices/PCI/pci.h>
#include <libc/stdiox.h>

extern pci_device* ne2000_pci_info_get();
extern uint16_t ne2000_vendorid_get();

void nic_info(void)
{

    pci_device* nic_info = ne2000_pci_info_get();

    // xprintf("bus: 0x%X\n", nic_info->bus);
    // xprintf("slot: 0x%X\n", nic_info->slot);
    // xprintf("function: 0x%X\n", nic_info->function);
    // xprintf("offset: 0x%X\n", nic_info->offset);
    // xprintf("vendor id: 0x%X\n", nic_info->vendor_id);
    // xprintf("device id: 0x%X\n", nic_info->device_id);
    
    xprintf("base: 0x%X\n", nic_info->base0);
    xprintf("base1: 0x%X\n", nic_info->base1);
    xprintf("base2: 0x%X\n", nic_info->base2);
    xprintf("base3: 0x%X\n", nic_info->base3);
    xprintf("base4: 0x%X\n", nic_info->base4);
    xprintf("base5: 0x%X\n", nic_info->base5);

    xprintf("subsystem id: 0x%x\n", nic_info->subsystem_id);

    // xprintf("0x%x\n", inb)

    // xprintf("str: %x\n", *(uint32_t*)nic_info->base0);
    // xprintf("str: %x\n", *(uint32_t*)nic_info->base1);
    // xprintf("str: %x\n", *(uint32_t*)nic_info->base2);
    // xprintf("str: %x\n", *(uint32_t*)nic_info->base3);
    // xprintf("str: %x\n", *(uint32_t*)nic_info->base4);
    // xprintf("str: %x\n", *(uint32_t*)nic_info->base5);
    // xprintf("interrupt line: 0x%X\n", nic_info->interrupt_line);
    // xprintf("interrupt pin: 0x%X\n", nic_info->interrupt_pin);

    // xprintf("mac: 0x");

    // for(int i = 0; i < 6; i++)
    // {
    //     uint8_t* tmp = nic_info->base4;
    //     xprintf("%mx", tmp[i]);
    // }

    while(getscan() != ENTER);



}

