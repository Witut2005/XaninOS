#pragma once

#include <lib/hal.h>
#include <devices/PCI/pci.h>

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


uint32_t pci_get_data32(uint32_t configuration_address, uint8_t register_id) 
{


    pci_set_parameters(pci_config_address, configuration_address);



    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);




    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(inddIO(PCI_DATA_PORT));
   

    return ret;


}




uint16_t pci_get_data16(uint32_t configuration_address, uint8_t register_id,
                                                    uint8_t register_offset)
{

    pci_set_parameters(pci_config_address, configuration_address);



    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC) | 
                        (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);


    outddIO(PCI_ADDRESS_PORT, address);
    ret = (uint16_t)(inddIO(PCI_DATA_PORT) >> ((register_offset & 0x3) * 8));
   

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
    ret = (uint8_t)(inddIO(PCI_DATA_PORT) >> ((register_offset & 0x3) * 8));
   

    return ret;


}



uint32_t pci_write_data32(uint32_t configuration_address, uint8_t register_id, uint32_t value)
{

    pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC));


    outddIO(PCI_ADDRESS_PORT, address);
    outddIO(PCI_DATA_PORT,value);
    //ret = inddIO(PCI_DATA_PORT);

    ret = inddIO(PCI_DATA_PORT);

    return ret;

}


uint16_t pci_write_data16(uint32_t configuration_address, uint8_t register_id, 
                                        uint8_t register_offset, uint16_t value)
{

    pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address; 
    uint16_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC));


    address += register_offset;

    outddIO(PCI_ADDRESS_PORT, address);
    outddIO(PCI_DATA_PORT,value);
    ret = inddIO(PCI_DATA_PORT);

    return ret;

}



uint8_t pci_write_data8(uint32_t configuration_address, uint8_t register_id, 
                                        uint8_t register_offset,uint8_t value)
{

    pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address; 
    uint8_t ret;
     
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (register_id & 0xFC));


    address += register_offset;

    outddIO(PCI_ADDRESS_PORT, address);
    outbIO(PCI_DATA_PORT,value);
    
    ret = inddIO(PCI_DATA_PORT);

    return ret;
        

}



