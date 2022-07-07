
#include <devices/NIC/ne2000.hpp>
#include <libcpp/cstdio.h>

Ne2000Manager::Ne2000Manager()
{

    /*
    this->iobase = pci_device_class_detect(NE2000_PCI_CLASS);
    xprintf("ne2000 iobase detected: 0x%x\n", this->iobase);
    */
};

void Ne2000Manager::init()
{
    /* zrob w PIC.C wszystko po kolei bez zadnych uin32_t i bla bla bla */
    /* bad */
    this->pci_selector = pci_find_device(NE2000_PCI_CLASS, &pci_info);

    this->pci_info.base0 = this->pci_info.base0 & ~ 0x3;
    
    /*
    this->pci_info.bus = this->pci_selector & 0xFF;
    this->pci_info.function = this->pci_selector >> 8 & 0x7;
    this->pci_info.device_id = this->pci_selector >> 11 & 0x1F;
    this->pci_info.bus = this->pci_selector >> 16;
    */

    /* this is ok 
    this->pci_info.vendor_id = pci_get_data16(this->pci_info.bus, this->pci_info.slot,
                                                this->pci_info.function, 0x0);
    this->pci_info.device_id = pci_get_data16(this->pci_info.bus, this->pci_info.slot,
                                                this->pci_info.function, 0x2);
    */
}

pci_device* Ne2000Manager::pci_info_get()
{
    return &this->pci_info;
}

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
    //return pci_get_data16(this->pci_info.bus, this->pci_info.slot, this->pci_info.function, 0x0);
    return this->pci_info.vendor_id;

}
