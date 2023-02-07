
#include <devices/PCI/pci.h>
#include <devices/USB/usb.h>

int usb_controller_info(void)
{

    pci_device* USBController = (pci_device*)calloc(sizeof(pci_device));
    usb_controller_detect(USBController);

    xprintf("0x%x\n", USBController->_class);
    while(inputg().scan_code != ENTER);

}