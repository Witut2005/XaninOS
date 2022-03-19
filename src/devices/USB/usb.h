
#include <stdint.h>

enum usb_controller_types
{
    UHCI = 0x0,
    OHCI = 0x10, 
    EHCI = 0x20,
    XHCI = 0x30
};



uint32_t pci_usb_id[10]; /* you can use this array to store pointers to PCI_CONFIG_SPACE */







