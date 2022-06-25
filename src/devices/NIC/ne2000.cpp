
#include <devices/NIC/ne2000.hpp>
#include <libcpp/cstdio.h>

Ne2000Manager::Ne2000Manager()
{

    /*
    this->iobase = pci_device_class_detect(NE2000_PCI_CLASS);
    xprintf("ne2000 iobase detected: 0x%x\n", this->iobase);
    */
};


uint32_t Ne2000Manager::mac_get()
{



}

uint32_t Ne2000Manager::iobase_get()
{
    /*
    this->pci_selector = pci_device_class_detect(NE2000_PCI_CLASS);
    return this->pci_selector;
    */
}

uint16_t Ne2000Manager::vendorid_get()
{
    return pci_get_vendor_id(this->pci_selector >> 16, this->pci_selector >> 11 & 0x1F, this->pci_selector >> 8 & 0x7);
}