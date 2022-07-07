
#include <devices/PCI/pci.h>
#include <libc/stdiox.h>

extern pci_device* ne2000_pci_info_get();
extern uint16_t ne2000_vendorid_get();

void nic_info(void)
{

    pci_device* nic_info = ne2000_pci_info_get();

    xprintf("bus: 0x%x\n", nic_info->bus);
    xprintf("slot: 0x%x\n", nic_info->slot);
    xprintf("function: 0x%x\n", nic_info->function);
    xprintf("offset: 0x%x\n", nic_info->offset);
    xprintf("vendor id: 0x%x\n", nic_info->vendor_id);
    xprintf("device id: 0x%x\n", nic_info->device_id);
    xprintf("base: 0x%x\n", nic_info->base0);
    xprintf("interrupt line: 0x%x\n", nic_info->interrupt_line);
    xprintf("interrupt pin: 0x%x\n", nic_info->interrupt_pin);

    while(getscan() != ENTER);



}

