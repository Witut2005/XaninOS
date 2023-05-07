
#include <sys/devices/nic/ne2000.h>

void Ne2000::init()
{
    this->pci_selector = pci_find_device(NE2000_CLASS_ID, NE2000_DEVICE_ID, &this->device_info);
    
}