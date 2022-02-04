
#include <devices/USB/usb.h>
#include <devices/PCI/pci.h>


uint8_t usb_controller_get_type(uint32_t configuration_address)
{

    pci_set_parameters(pci_config_address,configuration_address);

    uint32_t address;  
    uint8_t ret = 0; 
 
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (0x8) | (uint32_t) ENABLE_CONFIGURATION_SPACE_MAPPING);


    outddIO(PCI_ADDRESS_PORT, address);

    ret = (uint8_t)((inddIO(PCI_DATA_PORT) >> (1 * 8)) & 0xFF);
    return ret;

}

char* usb_controller_names[4] = 
{

    "UHCI",
    "OHCI",
    "EHCI",
    "XHCI"

};


void uhci_init(uint32_t configuration_address, uint8_t controller_type)
{

    pci_set_parameters(pci_config_address, configuration_address);

    uint32_t address;

    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (0x0 & 0xFC));


    if(controller_type != UHCI)
        return;
    else
    {
        pci_write_data32(configuration_address, 0x34, (uint32_t)0x0);
        pci_write_data32(configuration_address, 0x38, (uint32_t)0x0);
        pci_write_data8(configuration_address, 0x3C, 0x0, 0x23);
        pci_write_data16(configuration_address, 0x4, 0x0, 0x5);
        pci_write_data16(configuration_address, 0xC, 0x0, 0x8F00);




    }


}



