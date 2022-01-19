
#include <devices/PCI/pci.h>

uint16_t pci_get_vendor_id(uint8_t bus_number, uint8_t device_number, uint8_t function_number, uint8_t data_offset)
{
    uint32_t tmp;
    uint16_t return_val;

    /* data_offset must be on DWORD boundary */
    tmp = ENABLE_CONFIGURATION_SPACE_MAPPING | (bus_number << 16) | (device_number << 11) | (function_number << 8) | (data_offset & 0xFC); 
    outddIO(PCI_ADDRESS_PORT,tmp);

    return_val = (inddIO(PCI_DATA_PORT) >> ((data_offset & 2) << 3)) & 0xffff;
    return return_val;
}