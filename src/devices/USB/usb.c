
#include <devices/USB/usb.h>
#include <devices/PCI/pci.h>
#include <libc/stdiox.h>

char* usb_controller_names[4] = 
{
    "UHCI",
    "OHCI",
    "EHCI",
    "XHCI"
};

/*
uint8_t usb_controller_get_type(const uint8_t device, const uint8_t slot, const uint8_t function, )
{


    uint32_t address;  
    uint8_t ret = 0; 
 
    address = (uint32_t)((pci_config_address.pci_bus_number << 16) | 
                        (pci_config_address.pci_device_number << 11) |
                        (pci_config_address.pci_function_number << 8)|
                        (0x8) | (uint32_t) ENABLE_CONFIGURATION_SPACE_MAPPING);


    outdIO(PCI_ADDRESS_PORT, address);

    ret = (uint8_t)((indIO(PCI_DATA_PORT) >> (1 * 8)) & 0xFF);
    return ret;

}
*/
/*
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
        //pci_write_data8(configuration_address, 0x3C, 0x0, 0x23);
        pci_write_data32(configuration_address, 0x4, 0x5);
        pci_write_data32(configuration_address, 0xC, 0x8F00);
    }


}
*/


void usb_detect(void)
{
    uint32_t pci_address_selector = 0x0;


    for(pci_address_selector = 0x0; pci_address_selector < 2500000; pci_address_selector+=0x4) 
    {
        static uint16_t var, tmp; 
        tmp = var;

        var = pci_get_data16((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16, 
                                            (pci_address_selector & 0xFF00) >> 8, 0xa);

 
        if(var == 0x0c03 && tmp != var)
        {


            xprintf("USB CONTROLLER DETECTED VENDOR ID: ");
            xprintf("0x%x\n",pci_get_data16((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16, 
                                                                                        (pci_address_selector & 0xFF00) >> 8, 0x0));          
            
            xprintf("USB CONTROLLER TYPE: %s\n", 
                    usb_controller_names[pci_get_data8((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16,
                                                                                            (pci_address_selector & 0xFF00) >> 8, 0x9) / 0x10]);
            xprintf("USB CONTROLLER BASE ADDRES: ");

            if(!pci_get_data8((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16,
                                                                                            (pci_address_selector & 0xFF00) >> 8, 0x9) / 0x10)
                xprintf("0x%x\n",pci_get_data32((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16, (pci_address_selector & 0xFF00) >> 8, 0x20));  
            

            else
            xprintf("0x%x\n",pci_get_data32((pci_address_selector & 0xFF000000) >> 24, (pci_address_selector & 0xFF0000) >> 16,
                                                                                                            (pci_address_selector & 0xFF00) >> 8, 0x10));  

            return;

        }
           
   

    }

}
