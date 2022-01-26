

#include <lib/hal.h>
#include <devices/PCI/pci.h>

/*uint16_t pci_get_vendor_id(uint8_t bus_number, uint8_t device_number, uint8_t function_number, uint8_t data_offset)
{
    uint32_t tmp;
    uint16_t return_val;

     data_offset must be on DWORD boundary 
    tmp = ENABLE_CONFIGURATION_SPACE_MAPPING | (bus_number << 16) | (device_number << 11) | (function_number << 8) | (data_offset & 0xFC); 
    outddIO(PCI_ADDRESS_PORT,tmp);

    return_val = (inddIO(PCI_DATA_PORT) >> ((data_offset & 2) << 3)) & 0xffff;
    return return_val;
}
*/


uint16_t pci_get_vendor_id(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
 
    // Write out the address
    outddIO(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((inddIO(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}


//uint16_t detect_device_class()
