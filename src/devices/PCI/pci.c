#pragma once

#include <libc/hal.h>
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


uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t bar_number)
{
    

    uint32_t address; 
    uint32_t ret;
     
    address = (uint32_t)((bus << 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (0x10 + 0x4 * bar_number) | (uint32_t)ENABLE_CONFIGURATION_SPACE_MAPPING);

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


uint8_t pci_get_data8(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function, const uint8_t offset )
{

    uint32_t address; 
    uint8_t ret;
     
    address = (uint32_t)((bus<< 16) | 
                        (slot << 11) |
                        (function << 8)|
                        (offset & 0xFC) | 
                        ENABLE_CONFIGURATION_SPACE_MAPPING);


    outdIO(PCI_ADDRESS_PORT, address);
    ret = (uint8_t)(indIO(PCI_DATA_PORT) >> ((offset & 0x3) * 8)) & 0xFF;
   

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
                        (offset & 0xFC) | ENABLE_CONFIGURATION_SPACE_MAPPING);

    

    outdIO(PCI_ADDRESS_PORT, address);
    outdIO(PCI_DATA_PORT, (indIO(PCI_DATA_PORT) & ~(0x000000FF << ((offset & 0x3) * 8))) | (data << ((offset & 0x3) * 8)));
     
    outdIO(PCI_ADDRESS_PORT, address);
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


uint32_t pci_find_device(uint16_t class, pci_device* device_data)
{



    uint32_t pci_address_selector = 0x0;

    uint32_t* device_tmp = (uint32_t*)device_data;

    for(pci_address_selector = 0x0; pci_address_selector < 2500000; pci_address_selector+=0x4) 
    {
        static uint16_t var, tmp; 
        tmp = var;

        var = pci_get_data16((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16, 
                                            (pci_address_selector & 0xFF00) >> 8, 0xa);

 
        if(var == class && tmp != var)   
        {
        

            for(int i = 0x0; i <= 0xF; i++)
                *(device_tmp + i) = pci_get_data32((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16, 
                                                    (pci_address_selector & 0xFF00) >> 8, 4 * i);
            
            device_data->offset = pci_address_selector & 0xFF;
            device_data->function = (pci_address_selector >> 8) & 0x7;
            device_data->slot = (pci_address_selector >> 11) & 0xF;
            device_data->bus = (pci_address_selector >> 16) & 0xFF;

            return pci_address_selector;
        
        }

    }

    return 0xFFFFFFFF;

}

