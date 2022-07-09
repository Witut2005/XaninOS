
#include <devices/NIC/8254x.hpp>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/chal.h>
#include <libcpp/ostream.h>


void Intel8254x::write(uint32_t ioaddr, uint32_t value)
{
    *(this->ioaddr) = ioaddr;
    *(this->iodata) = value;
}

#define reset() write(0x0, 0x4000000)

void Intel8254x::init()
{

    this->pci_selector = pci_find_device(NE2000_PCI_CLASS, &pci_info);
    
    this->iobase = (uint32_t*)this->pci_info.base0;

    this->ioaddr = this->iobase + nic::IOADDR;
    this->iodata = this->iobase + nic::IODATA;

    std::cout << std::clear;

    std::cout << std::hex << this->iobase << std::endl;

    reset();
    
    std::cout << "Current value test:" << std::hex << *iobase << std::endl;




}

pci_device* Intel8254x::pci_info_get()
{
    return &this->pci_info;
}

uint32_t Intel8254x::mac_get()
{



}

uint32_t Intel8254x::iobase_get()
{
    return pci_info.base0;
}

uint16_t Intel8254x::vendorid_get()
{
    return this->pci_info.vendor_id;
}
