
#include <sys/devices/pci/pci.h>
#include <sys/devices/usb/usb.h>

int usb_controller_info(void)
{

    pci_device* USBController = (pci_device*)calloc(SIZE_OF(pci_device));
    usb_controller_detect(USBController);

    xprintf("0x%x\n", USBController->_class);
    while(inputg().scan_code != ENTER);

    return XANIN_OK;

}