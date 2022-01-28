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


uint16_t pci_get_vendor_id(uint32_t configuration_address)
{ 
    
    pci_set_parameters(pci_config_address,configuration_address);

    uint32_t address;  
    uint16_t ret = 0; 
 
  
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) | 
                        (pci_config_address.pci_function_number << 8) |
                        (0x0) | ((uint32_t)0x80000000));

    outddIO(PCI_ADDRESS_PORT, address);

    ret = (uint16_t)(inddIO(PCI_DATA_PORT) & 0xFFFF);
    return ret;
}



uint8_t pci_get_device_data(uint32_t configuration_address) 
{ 
    

    pci_set_parameters(pci_config_address,configuration_address);

    uint32_t address;  
    uint8_t ret = 0; 
 
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (pci_config_address.pci_register_offset));


    outddIO(PCI_ADDRESS_PORT, address);

    ret = (uint32_t)((inddIO(PCI_DATA_PORT) >> (1 * 8)) & 0xFF);
    return ret;
}

uint16_t pci_get_device_class(uint32_t configuration_address) 
{

    pci_set_parameters(pci_config_address, configuration_address);


    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (0x8) | (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);

    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(inddIO(PCI_DATA_PORT) >> 16);
    

    return ret;

}


uint32_t pci_get_bar(uint32_t configuration_address)
{
    
    pci_set_parameters(pci_config_address, configuration_address);



    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (0x20) | (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);

    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint32_t)(inddIO(PCI_DATA_PORT));
   

    return ret;



}


uint32_t pci_get_data32(uint32_t configuration_address, uint32_t register_offset)
{


    pci_set_parameters(pci_config_address, configuration_address);



    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_offset & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);




    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(inddIO(PCI_DATA_PORT) >> (register_offset & 0xFC) );
   

    return ret;


}




uint16_t pci_get_data16(uint32_t configuration_address, uint8_t register_offset)
{


    pci_set_parameters(pci_config_address, configuration_address);



    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_offset & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);


    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(inddIO(PCI_DATA_PORT) >> (register_offset & 0xFC) );
   

    return ret;


}


uint16_t pci_get_data8(uint32_t configuration_address, uint8_t register_id, 
                                                    uint8_t register_offset)
{


    pci_set_parameters(pci_config_address, configuration_address);



    uint32_t address; 
    uint8_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);


    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint8_t)(inddIO(PCI_DATA_PORT) >> ((register_offset & 0xFC) * 8));
   

    return ret;


}






