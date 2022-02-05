#pragma once

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC


#define ENABLE_CONFIGURATION_SPACE_MAPPING 0x80000000



 

struct pci_config_register
{

    uint8_t pci_bus_number;
    uint8_t pci_device_number;
    uint8_t pci_function_number;
    uint8_t pci_register_offset;

}pci_config_address;

typedef struct pic_config_register pci_config_register;


#define pci_set_parameters(config_register, parameter) \
    config_register.pci_bus_number = (parameter & 0xFF0000) >> 16;\
    config_register.pci_device_number = (parameter & 0xF800) >> 11;\
    config_register.pci_function_number = (parameter & 0x700) >> 8;\
    config_register.pci_register_offset = (parameter & 0xFF)

enum PCI_DEVICES
{
   USB_CLASS_CONTROLLER_CODE = 0x0C,
   USB_SUBCLASS_CONTROLLER_CODE = 0x03,
 
   NETWORK_CLASS_CONTROLLER_CODE = 0x2,
   ETHERNET_SUBCLASS_CONTROLLER_CODE = 0x0
};


char* pci_device_name[15] = 
{
    "unclassified",
    "mass storage controller",
    "network controller",
    "display controller",
    "multimedia controller",
    "memory controller",
    "bridge",
    "simple communication controller",
    "base system peripheral",
    "input decide controller",
    "docking station",
    "processor",
    "serial bus controller"

};

void pci_write_data32(uint32_t configuration_address, uint8_t register_id, uint32_t value);




