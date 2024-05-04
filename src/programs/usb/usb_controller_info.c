
#include <lib/libc/stdiox.h>
#include <sys/devices/pci/pci.h>
#include <sys/devices/usb/usb.h>
// TERMINAL_APP

int usb_controller_info(void)
{

    pci_device* USBController = (pci_device*)calloc(sizeof(pci_device));
    usb_controller_detect(USBController);

    xprintf("0x%x\n", USBController->_class);

    return XANIN_OK;
}
