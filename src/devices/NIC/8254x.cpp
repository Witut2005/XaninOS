
#include <devices/NIC/8254x.hpp>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/chal.h>
#include <libcpp/ostream.h>

#define reset() write(0x0, 0x4000000)

void Intel8254xDriver::write(uint32_t reg, uint32_t value)
{
    *(uint32_t*)(this->iobase + reg) = value;
    io_wait();
}

uint32_t Intel8254xDriver::read(uint32_t reg)
{
    return *(uint32_t*)(this->iobase + reg); 
}

uint16_t Intel8254xDriver::eeprom_read(uint8_t address)
{

    uint32_t address32 = static_cast<uint32_t>((address << 8));

    address32 = address32 | 0x1;

    this->write(nic::EERD, address32);
    io_wait();
    
    uint32_t ret = this->read(nic::EERD);

    ret = ret >> 16;

    address32 = address32 ^ 0x1;

    this->write(nic::EERD, address32);

    return ret;

}


void Intel8254xDriver::init()
{

    std::cout << std::clear;
    
    /* finding device */
    this->pci_selector = pci_find_device(INTEL_8254X, &pci_info);
    this->iobase = (uint8_t*)this->pci_info.base0;


    /* setting pci command register */
    uint16_t pci_command = pci_get_data16(pci_info.bus, pci_info.slot, pci_info.function, 0x4);
    pci_write_data16(pci_info.bus, pci_info.slot, pci_info.function, 0x4, pci_command | 0x7); 

    /* device reset */
    reset();

    this->write(nic::EECD, this->read(nic::EECD) | nic::EECD_SK | nic::EECD_CS | nic::EECD_DI);

    /* enable reading from eeprom */
    this->write(nic::EECD, this->read(nic::EECD) | nic::EECD_REQ);
    while( (this->read(nic::EECD) & nic::EECD_GNT) >> 7 != 1);
    this->write(nic::EECD, this->read(nic::EECD) ^ nic::EECD_REQ);
    

    /* reading mac address */
    *(uint16_t*)&mac[0] = this->eeprom_read(0x0);
    *(uint16_t*)&mac[2] = this->eeprom_read(0x1);
    *(uint16_t*)&mac[4] = this->eeprom_read(0x2);

    /* enable receiving packets */
    this->write(nic::RCTL, nic::RCTL_EN);

}

pci_device* Intel8254xDriver::pci_info_get()
{
    return &this->pci_info;
}

uint8_t* Intel8254xDriver::mac_get()
{
    return this->mac;
}

uint32_t Intel8254xDriver::iobase_get()
{
    return pci_info.base0;
}

uint16_t Intel8254xDriver::vendorid_get()
{
    return this->pci_info.vendor_id;
}
