
#include <devices/NIC/8254x.hpp>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/chal.h>
#include <libcpp/ostream.h>

#define INTEL_8254X_DESCRIPTORS 1024
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

void Intel8254xDriver::multicast_table_array_clear(void)
{
    for(int i = 0 ; i < 0x1FC; i += 4)
        this->write(nic::MTA + i, 0x0);
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


void Intel8254xDriver::receive_init(void)
{
    /* receive buffer allocation */
    this->write(nic::RDBAL, (uint32_t)malloc(sizeof(i8254xReceiveDescriptor) * INTEL_8254X_DESCRIPTORS));
    this->write(nic::RDBAH, 0x0);
    this->receive_buffer = (i8254xReceiveDescriptor*)(this->read(nic::RDBAL));
    
    for(uint8_t* i = (uint8_t*)this->read(nic::RDBAL); (uint32_t)i < this->read(nic::RDBAL) + INTEL_8254X_DESCRIPTORS * sizeof(i8254xReceiveDescriptor); i++)
        *i = 0x0;


    this->write(nic::RDLEN, INTEL_8254X_DESCRIPTORS * sizeof(i8254xReceiveDescriptor));

    /* set head and tail to proper values */
    this->write(nic::RDH, 0x0);
    this->write(nic::RDT, INTEL_8254X_DESCRIPTORS);

    /* set receive control register */
    // std::cout << std::hex << "SIZE BUFFER: " << this->read(nic::RCTL) << std::endl;
    this->write(nic::RCTL, this->read(nic::RCTL) & 0xFFFFFF3F); //LBM
    // std::cout << std::hex << "SIZE BUFFER: " << this->read(nic::RCTL) << std::endl;

    this->write(nic::RCTL, this->read(nic::RCTL) | nic::rctl::BAM); // accept broadcast packets
    
    this->write(nic::RCTL, this->read(nic::RCTL) | nic::rctl::BSEX);
    this->write(nic::RCTL, this->read(nic::RCTL) | (0x1 << 16));  // BSIZE

    /* enable receiving packets */
    this->write(nic::RCTL, this->read(nic::RCTL) | nic::RCTL_EN);

}

void Intel8254xDriver::transmit_init(void)
{


    /* transmit buffer allocation */
    this->transmit_buffer = (i8254xTransmitDescriptor*)malloc(sizeof(i8254xTransmitDescriptor) * INTEL_8254X_DESCRIPTORS);

    for(int i = 0; i < INTEL_8254X_DESCRIPTORS; i++)
    {
        this->transmit_buffer[i].address_low = 0x0;
        this->transmit_buffer[i].address_high = 0x0;
        this->transmit_buffer[i].length = 0x0;
        this->transmit_buffer[i].cso = 0x0;
        this->transmit_buffer[i].cmd = 0x0;
        this->transmit_buffer[i].status = 0x0;
        this->transmit_buffer[i].css = 0x0;
        this->transmit_buffer[i].special = 0x0;
    }

    std::cout << std::hex << (uint32_t)this->transmit_buffer << std::endl;
    std::cout << std::hex << this->transmit_buffer[100].address_low << std::endl;

    // while(1);

    this->write(nic::TDBAL, (uint32_t)this->transmit_buffer);
    this->write(nic::TDBAH, 0x0);
    // this->transmit_buffer = (i8254xTransmitDescriptor*)(this->read(nic::TDBAL));

    for(uint8_t* i = (uint8_t*)this->read(nic::TDBAL); (uint32_t)i < this->read(nic::TDBAL) + INTEL_8254X_DESCRIPTORS * sizeof(i8254xTransmitDescriptor); i++)
        *i = 0x0;

    /* set buffer length */
    this->write(nic::TDLEN, INTEL_8254X_DESCRIPTORS * 16);

    /* set head and tail to proper values */
    this->write(nic::TDH, 0x0);
    this->write(nic::TDT, INTEL_8254X_DESCRIPTORS);

    /* enable transmit packets */
    this->write(nic::TCTL, this->read(nic::TCTL) | nic::tctl::EN | nic::tctl::PSP);

    this->write(nic::TCTL, 0x10 << 4); //coliision treshold
    this->write(nic::TCTL, 0x40 << 12); //collision distance

    this->write(nic::TIPG, 10 << 0);
    this->write(nic::TIPG, 10 << 10);
    this->write(nic::TIPG, 10 << 20);




}


void Intel8254xDriver::init()
{

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
    while((this->read(nic::EECD) & nic::EECD_GNT) >> 7 != 1);
    this->write(nic::EECD, this->read(nic::EECD) ^ nic::EECD_REQ);
    
    this->mac_get();

    /* general configuration (page 389) */
    this->write(nic::CTRL, this->read(nic::CTRL) | nic::ctrl::ASDE | nic::ctrl::SLU); // step 2
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::LRST)); // step 5
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::PHY_RST)); // step 6
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::ILOS)); // step 7 (can be bad)

    this->multicast_table_array_clear(); // clear multicast table
    this->multicast_table_array_clear(); // clear multicast table
    
    /* disable VLANs */
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::VME));

    this->receive_init();
    this->transmit_init();


    /* enabling interrupts */
    this->write(nic::IMS, this->read(nic::IMS) | nic::ims::RXT | nic::ims::RXO | 
                  nic::ims::RXDMT | nic::ims::RXSEQ | nic::ims::LSC);
    

    while(1)
    {
        this->send_packet(0x0, 128);
    }

    // this->write(nic::IMS, 0x1F6DC);
    // this->write(nic::IMS, 0xFFFFFFF0);


}

uint32_t Intel8254xDriver::receive_buffer_get(void)
{
    return this->read(nic::RDBAL);
}

uint32_t Intel8254xDriver::transmit_buffer_get(void)
{
    return (uint32_t)this->transmit_buffer;
    // return this->read(nic::TDBAL);
}

pci_device* Intel8254xDriver::pci_info_get()
{
    return &this->pci_info;
}

uint8_t* Intel8254xDriver::mac_get()
{
    *(uint16_t*)&mac[0] = this->eeprom_read(0x0);
    *(uint16_t*)&mac[2] = this->eeprom_read(0x1);
    *(uint16_t*)&mac[4] = this->eeprom_read(0x2);
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

void Intel8254xDriver::receive_packet(void)
{
    while(this->receive_buffer[this->read(nic::RDT)].status & 0x1)
    {
        uint8_t* packet = (uint8_t*)this->receive_buffer[this->read(nic::RDT)].address_low;
        uint16_t packet_lenght = this->receive_buffer[this->read(nic::RDT)].length;
        xprintf("0x%x 0x%x", (uint32_t)packet, packet_lenght);
    }

}

void Intel8254xDriver::send_packet(uint32_t address, uint16_t length)
{
    this->transmit_buffer[this->read(nic::TDT)].address_low = address;
    this->transmit_buffer[this->read(nic::TDT)].address_high = 0x0;
    this->transmit_buffer[this->read(nic::TDT)].cmd = 0xF ^ 0x4;
    this->transmit_buffer[this->read(nic::TDT)].length = length;

    this->transmit_buffer[this->read(nic::TDT)].status = 0x0;
    this->transmit_buffer[this->read(nic::TDT)].css = 0x0;
    this->transmit_buffer[this->read(nic::TDT)].special = 0x0;

    uint32_t tdt_old = this->read(nic::TDT);
    this->write(nic::TDT, (this->read(nic::TDT) + 1) % INTEL_8254X_DESCRIPTORS);

    while(this->transmit_buffer[tdt_old].status & 0xF)    
    {
        msleep(1);
    }


}

void  Intel8254xDriver::interrupt_handler(void)
{
    uint16_t interrupt_status = this->read(nic::ICR);

    xprintf("ICR STATUS 0x%x", interrupt_status);

    while(1);

}
