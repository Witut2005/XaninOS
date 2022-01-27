#pragma once

#include <lib/hal.h>
#include <devices/PCI/pci.h>

/*uint16_t pci_get_vendor_id(uint8_t bus_number, uint8_t device_number, uint8_t function_number, uint8_t data_offset)
{
    uint32_t tmp;
    uint16_t return_val;

     data_offset must be on DWORD boundary   tmp = ENABLE_CONFIGURATION_SPACE_MAPPING | (bus_number << 16) | (device_number << 11) | (function_number << 8) | (data_offset & 0xFC); 
    outddIO(PCI_ADDRESS_PORT,tmp);

    return_val = (inddIO(PCI_DATA_PORT) >> ((data_offset & 2) << 3)) & 0xffff;
    return return_val;
}
*/


uint16_t pci_get_vendor_id(uint8_t bus, uint8_t slot, uint8_t function)
{ 
    
    uint32_t address;  
    uint16_t ret = 0; 
 
  
    address = (uint32_t)((bus << 16) | (slot << 11) | (function << 8) |
              (0x0) | ((uint32_t)0x80000000));

    outddIO(PCI_ADDRESS_PORT, address);

    ret = (uint16_t)(inddIO(PCI_DATA_PORT) & 0xFFFF);
    return ret;
}



uint8_t pci_get_device_data (uint8_t bus, uint8_t slot, uint8_t function, uint8_t register_offset)
{ 
    
    uint32_t address;  
    uint8_t ret = 0; 
 
  
    address = (uint32_t)((bus << 16) | (slot << 11) | (function << 8) |
              (register_offset & 0xFC) | ((uint32_t)0x80000000));

    outddIO(PCI_ADDRESS_PORT, address);

    ret = (uint32_t)((inddIO(PCI_DATA_PORT) >> (1 * 8)) & 0xFF);
    return ret;
}

uint16_t pci_get_device_class(uint8_t bus, uint8_t slot, uint8_t function) 
{

    uint32_t address; 
    uint16_t ret;
    
    address = (uint32_t)((bus << 16) | (slot << 11) | (function << 8) |
              (0x8) | ((uint32_t)0x80000000));

    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(inddIO(PCI_DATA_PORT) >> 16);
    

    return ret;

}

//uint16_t detect_device_class()
