
#include <devices/NIC/8254x.hpp>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/chal.h>
#include <libcpp/ostream.h>

#define reset() write(0x0, 0x4000000)

void Intel8254x::write(uint32_t reg, uint32_t value)
{
    *(uint32_t*)(this->iobase + reg) = value;
    io_wait();
}

uint32_t Intel8254x::read(uint32_t reg)
{
    return *(uint32_t*)(this->iobase + reg); 
}

uint16_t Intel8254x::eeprom_read(uint8_t address)
{

    uint32_t address32 = static_cast<uint32_t>((address & 0xFFF) << 2);

    address32 = address32 | 0x1;

    this->write(nic::EERD, address32);
    io_wait();
    
    uint16_t ret = this->read(nic::EERD);

    address32 = address32 ^ 0x1;

    this->write(nic::EERD, address32);

    return ret;

}


void Intel8254x::init()
{

    this->pci_selector = pci_find_device(INTEL_8254X, &pci_info);
    
    this->iobase = (uint8_t*)this->pci_info.base0;


    // std::cout << std::clear;
    // std::cout << "CONTROL STATE:" << std::hex << this->read(nic::CTRL) << std::endl;
    // // outdIO(0xC000, 0x0);
    // std::cout << "CONTROL STATE:" << std::hex << indIO(0xC001) << std::endl;
    // std::cout << "CONTROL STATE:" << std::hex << this->read(nic::CTRL) << std::endl;
    // // std::cout << std::hex << this->iobase << std::endl;

    // reset();

    // this->write(nic::EECD, this->read(nic::EECD) | nic::EECD_SK | nic::EECD_CS | nic::EECD_DI);

    // this->write(nic::EECD, this->read(nic::EECD) | nic::EECD_REQ);
    // while( (this->read(nic::EECD) & nic::EECD_GNT) >> 7 != 1);
    // this->write(nic::EECD, this->read(nic::EECD) ^ nic::EECD_REQ);

    // // std::cout << "CONTROL STATE:" << std::hex << this->read(nic::CTRL) << std::endl;
    
    // std::cout << std::hex << this->eeprom_read(0x0) << std::endl;
    // std::cout << std::hex << this->eeprom_read(0x1) << std::endl;
    // std::cout << std::hex << this->eeprom_read(0x2) << std::endl;

    // while(1);

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
