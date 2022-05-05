#pragma once

#include <lib/hal.h>
#include <devices/PCI/pci.h>

uint16_t pci_get_vendor_id(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function)
{ 
    

    uint32_t address;  
    uint16_t ret = 0; 
 
  
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) | 
                        (function << 8) |
                        (0x0) | ((uint32_t)0x80000000));

    outdIO(PCI_ADDRESS_PORT, address);

    ret = (uint16_t)(indIO(PCI_DATA_PORT) & 0xFFFF);
    return ret;
}


uint8_t pci_get_device_data(const uint8_t bus, const uint8_t slot,
                            const uint8_t function, const uint8_t offset) 
{ 
    


    uint32_t address;  
    uint8_t ret = 0; 
 
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (offset));


    outdIO(PCI_ADDRESS_PORT, address);

    ret = (uint32_t)((indIO(PCI_DATA_PORT) >> (1 * 8)) & 0xFF);
    return ret;
}

uint16_t pci_get_device_class(const uint8_t bus, const uint8_t slot,
                                const uint8_t function) 
{



    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (0x8) | (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);

    outdIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(indIO(PCI_DATA_PORT) >> 16);
    

    return ret;

}


uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function)
{
    

    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (0x20) | (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);

    outdIO(PCI_ADDRESS_PORT, address);
    ret = (uint32_t)(indIO(PCI_DATA_PORT));
   

    return ret;



}


uint32_t pci_get_data32(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function, const uint8_t offset) 
{


    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (offset & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);




    outdIO(PCI_ADDRESS_PORT, address);
    ret = (indIO(PCI_DATA_PORT));
   

    return ret;


}




uint16_t pci_get_data16(const uint8_t bus, const uint8_t slot,
                        const uint8_t function, const uint8_t offset)
{

    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (offset & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);


    outdIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(indIO(PCI_DATA_PORT) >> ((offset & 0x3) * 8));
   
    return ret;


}


uint16_t pci_get_data8(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function, const uint8_t offset )
{

    uint32_t address; 
    uint8_t ret;
     
    address = (uint32_t)((bus<< 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (offset & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);


    outdIO(PCI_ADDRESS_PORT, address);
    ret = (uint8_t)(indIO(PCI_DATA_PORT) >> ((offset & 0x3) * 8));
   

    return ret;


}


uint8_t pci_write_data8(const uint8_t bus, const uint8_t slot, const uint8_t function, 
                                                const uint8_t offset, const uint8_t data)
{

    //pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address; 
    uint8_t ret;
     
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (offset & 0xFC));

    

    outdIO(PCI_ADDRESS_PORT, address);
    outdIO(PCI_DATA_PORT, (indIO(PCI_DATA_PORT) & ~(0x000000FF << (offset & 0x3))) | (data << (offset & 0x3)));
     
    ret = indIO(PCI_DATA_PORT);

    return ret;
        

}

void pci_write_data32(uint32_t configuration_address, uint8_t register_id, uint32_t value)
{

    //pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address; 
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC) | (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);

    outdIO(PCI_ADDRESS_PORT, address);
    outdIO(PCI_DATA_PORT,value);
}


uint16_t pci_write_data16(uint32_t configuration_address, uint8_t register_id, 
                                        uint8_t register_offset, uint16_t value)
{

    //pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC));


    address += register_offset;

    outdIO(PCI_ADDRESS_PORT, address);
    outdIO(PCI_DATA_PORT,value);
    ret = indIO(PCI_DATA_PORT);
            

    return ret;

}


uint32_t capabilities_pointer_get(void)
{

}
