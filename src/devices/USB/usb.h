
#pragma once

#include <stdint.h>
#include <devices/PCI/pci.h>

enum usb_controller_types
{
    UHCI = 0x0,
    OHCI = 0x10, 
    EHCI = 0x20,
    XHCI = 0x30
};

void usb_controller_detect(pci_device* USBConfigurationSpace);






