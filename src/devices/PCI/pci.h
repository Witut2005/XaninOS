

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC


#define ENABLE_CONFIGURATION_SPACE_MAPPING 0x80000000

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






