
#include <lib/libc/stdiox.h>
#include <sys/devices/pci/pci.c>


int get_device_info(void)
{


    uint32_t pci_selector = 0x0;

    // do
    // {

    

/*        uint16_t device_type = pci_get_device_class(((pci_selector & 0xFF0000) >> 16),((pci_selector & 0xF800) >> 11), ((pci_selector & 0x700) >> 8));
      pci_selector+=4;

        xprintf("%s\n", pci_device_name[device_type]);
*/
    // }while(pci_selector);

    return XANIN_OK;

}

