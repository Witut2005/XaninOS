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

    for(pci_address_selector = 0x0; pci_address_selector < 2500000; pci_address_selector++) 
    {
        static uint16_t var, tmp; 
        tmp = var;

        var = pci_get_data16((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16, 
                                            (pci_address_selector & 0xFF00) >> 8, 0xa);

 
        if(var == class && tmp != var)   
        {
        
            device_data->offset   = pci_address_selector & 0xFF;
            device_data->function = pci_address_selector >> 8 & 0x7;
            device_data->slot     = pci_address_selector >> 11 & 0x1F;
            device_data->bus      = pci_address_selector >> 16 & 0xFF;

            device_data->vendor_id = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x0);

            device_data->device_id = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x2);


            device_data->command = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x4);


            device_data->status = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x6);


            device_data->revision_id = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x8);


            device_data->p_interface = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x9);


            device_data->subclass = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0xA);


            device_data->_class = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0xb);


            device_data->cache_line_size = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0xC);


            device_data->latency_timer = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0xD);


            device_data->header_type = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0xE);


            device_data->bist = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0xF);

            device_data->base0 = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x10);


            device_data->base1 = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x14);


            device_data->base2 = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x18);

            device_data->base3 = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x1C);

            device_data->base4 = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x20);
                                                        
            device_data->base5 = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x24);

            device_data->card_bus = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x28);

            device_data->subsystem_vendor_id = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x2C);

            device_data->subsystem_id = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x2C + 0x2);


            device_data->rom_base = pci_get_data32(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x30);

            device_data->capabilities_pointer = pci_get_data16(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x34);
                                                        
            device_data->interrupt_line = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x3C);

            device_data->interrupt_pin = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x3C + 0x1);

            device_data->minimal_grant = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x3C + 0x2);

            device_data->max_latency = pci_get_data8(device_data->bus, device_data->slot, 
                                                        device_data->function, 0x3C + 0x3);

            return pci_address_selector;
        
        }

    }

    return 0xFFFFFFFF;

}

