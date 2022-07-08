
#include <devices/NIC/ne2000.hpp>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/ostream.h>

#define NE2000_INTERRUPT_OFF 0xFF

Ne2000Manager::Ne2000Manager()
{

    /*
    this->iobase = pci_device_class_detect(NE2000_PCI_CLASS);
    xprintf("ne2000 iobase detected: 0x%x\n", this->iobase);
    */
};

void Ne2000Manager::init()
{

    this->pci_selector = pci_find_device(NE2000_PCI_CLASS, &pci_info);
    this->pci_info.base0 = this->pci_info.base0 & ~ 0x3;
    
    hal::Port8 ResetPort(this->pci_info.base0 + 0x18);
    hal::Port8 Isr(this->pci_info.base0 + 0x7);

    hal::Port8 CommandRegister(this->pci_info.base0 + 0x0);

    hal::Port8 TransmitConfigPort(this->pci_info.base0 + 0xD);

    ResetPort.write(ResetPort.read());
    while(Isr.read() & 0x80 == 0);      // reseting device

    Isr.write(NE2000_INTERRUPT_OFF); 
    std::cout << std::clear;

    CommandRegister.write(0x80);

    std::cout << "Current value test:" << std::hex << static_cast<uint16_t>(TransmitConfigPort.read()) << std::endl;

    // while(1);

    //PageSelector.write()


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
    return pci_info.base0;
}

uint16_t Ne2000Manager::vendorid_get()
{
    //return pci_get_data16(this->pci_info.bus, this->pci_info.slot, this->pci_info.function, 0x0);
    return this->pci_info.vendor_id;

}
