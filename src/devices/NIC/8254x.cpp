
#include <devices/NIC/8254x.hpp>
#include <libcpp/cstdio.h>
#include <libcpp/hal.h>
#include <libcpp/chal.h>
#include <libcpp/ostream.h>
#include <libc/syslog.h>
#include <libcpp/cmemory.h>
#include <devices/APIC/apic_registers.h>

#define INTEL_8254X_DESCRIPTORS 256
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

    this->receive_buffer = (i8254xReceiveDescriptor*)malloc(sizeof(i8254xReceiveDescriptor) * INTEL_8254X_DESCRIPTORS);

    for(int i = 0; i < INTEL_8254X_DESCRIPTORS; i++)
    {
        this->receive_buffer[i].address_low = (uint32_t)&this->transmit_buffer[i];
        this->receive_buffer[i].address_high = 0x0;
        this->receive_buffer[i].length = 0x0;
        this->receive_buffer[i].checksum = 0x0;
        this->receive_buffer[i].status = 0x0;
        this->receive_buffer[i].errors = 0x0;
        this->receive_buffer[i].special = 0x0;
    }

    /* receive buffer allocation */
    this->write(nic::RDBAL, (uint32_t)this->receive_buffer);
    this->write(nic::RDBAH, 0x0);

    
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

    this->rxd_current = 0x0;

    /* enable receiving packets */
    this->write(nic::RCTL, this->read(nic::RCTL) | nic::RCTL_EN);

}

void Intel8254xDriver::transmit_init(void)
{


    /* transmit buffer allocation */
    // auto AHA = (i8254xTransmitDescriptor*)malloc(sizeof(i8254xTransmitDescriptor) * INTEL_8254X_DESCRIPTORS); //+ 16);

    auto transmit_buffer_region = malloc(sizeof(i8254xTransmitDescriptor) * INTEL_8254X_DESCRIPTORS); // kompilator zawsze ma racje, prawda? 
    this->transmit_buffer = (i8254xTransmitDescriptor*)transmit_buffer_region;


    for(int i = 0; i < INTEL_8254X_DESCRIPTORS; i++)
    {
        this->transmit_buffer[i].address_low = (uint32_t)&this->transmit_buffer[i];
        this->transmit_buffer[i].address_high = 0x0;
        this->transmit_buffer[i].length = 0x0;
        this->transmit_buffer[i].cso = 0x0;
        this->transmit_buffer[i].cmd = 0x0;
        this->transmit_buffer[i].status = 0x0;
        this->transmit_buffer[i].css = 0x0;
        this->transmit_buffer[i].special = 0x0;
    }


    // while(1);

    this->write(nic::TDBAL, (uint32_t)transmit_buffer_region);//this->transmit_buffer);
    this->write(nic::TDBAH, 0x0);
    // this->transmit_buffer = (i8254xTransmitDescriptor*)(this->read(nic::TDBAL));

    /* set head and tail to proper values */
    this->write(nic::TDH, 0x0);
    this->write(nic::TDT, INTEL_8254X_DESCRIPTORS);

    /* set buffer length */
    this->write(nic::TDLEN, INTEL_8254X_DESCRIPTORS * sizeof(i8254xTransmitDescriptor));

    this->txd_current = 0x0;

    /* enable transmit packets */
    this->write(nic::TCTL, this->read(nic::TCTL) | nic::tctl::EN | nic::tctl::PSP);

    // this->write(nic::TCTL, 0x10 << 4); //coliision treshold
    // this->write(nic::TCTL, 0x40 << 12); //collision distance

    this->write(nic::TIPG, 0x0060200A);





}

void Intel8254xDriver::send_packet(uint32_t address, uint16_t length)
{

    this->transmit_buffer[this->txd_current].address_low = address;
    this->transmit_buffer[this->txd_current].address_high = 0x0;
    this->transmit_buffer[this->txd_current].cmd = nic::CMD::EOP | nic::CMD::IFCS | nic::CMD::RS | nic::CMD::RPS;// | nic::CMD::RPS;
    this->transmit_buffer[this->txd_current].length = length;

    this->transmit_buffer[this->txd_current].status = 0x0;
    this->transmit_buffer[this->txd_current].css = 0x0;
    this->transmit_buffer[this->txd_current].cso = 0x0;
    this->transmit_buffer[this->txd_current].special = 0x0;


    auto txd_old = this->txd_current;
    this->write(nic::TDT,(this->txd_current + 1) % INTEL_8254X_DESCRIPTORS);
    this->txd_current = (this->txd_current + 1) % INTEL_8254X_DESCRIPTORS;

    while(!this->transmit_buffer[txd_old].status);


}

void Intel8254xDriver::send_ethernet_frame(uint8_t* mac_destination, uint8_t* mac_source, uint8_t* buffer, uint16_t length)
{
    uint8_t* tmp = (uint8_t*)malloc(1518);
    EthernetFrame* frame = (EthernetFrame*)tmp;
    

    memcpy(frame->mac_destination, mac_destination, 6);
    memcpy(frame->mac_source, mac_source, 6);

    frame->ethernet_type = 0x806; // ARP

    tmp = tmp + ETHERNET_FRAME_HEADER_SIZE;

    int i = 0;
    for(; i < length; i++)
    {
        tmp[i] = buffer[i];
    }

    tmp[i] = 0x0;
    tmp[i + 1] = 0x20;
    tmp[i + 2] = 0x20;
    tmp[i + 3] = 0x3A;

    this->send_packet((uint32_t)frame, length + ETHERNET_FRAME_HEADER_SIZE);
    free(frame);

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

    // /* general configuration (page 389) */
    // this->write(nic::CTRL, this->read(nic::CTRL) | nic::ctrl::ASDE | nic::ctrl::SLU); // step 2
    // this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::LRST)); // step 5
    // this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::PHY_RST)); // step 6
    // this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::ILOS)); // step 7 (can be bad)

    // this->multicast_table_array_clear(); // clear multicast table
    // this->multicast_table_array_clear(); // clear multicast table
    
    // /* disable VLANs */
    // this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::VME));

    this->receive_init();
    this->transmit_init();


    /* receive delay timer set */
    this->write(nic::RDTR, 0xFF | (1 << 31));


    /* enabling interrupts */
    this->write(nic::IMS, this->read(nic::IMS) | nic::ims::RXT | nic::ims::RXO | 
                  nic::ims::RXDMT | nic::ims::RXSEQ | nic::ims::LSC);
    
    // this->write(nic::RCTL, this->read(nic::RCTL) & (~nic::RCTL_EN));

    // while(1)
    // {
    //     this->send_packet(0x0, 128);
    // }


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

    this->rxd_current = this->read(nic::RDT) % INTEL_8254X_DESCRIPTORS;
    this->rxd_current = (this->rxd_current + 1) % INTEL_8254X_DESCRIPTORS;
    
    {
        uint8_t* packet = (uint8_t*)this->receive_buffer[rxd_current].address_low;
        uint16_t packet_lenght = this->receive_buffer[rxd_current].length;
    }

    this->write(nic::RDT, this->rxd_current);

}


void Intel8254xDriver::interrupt_handler(void)
{
    uint16_t interrupt_status = this->read(nic::ICR);
    
    if(interrupt_status & 0x80)
        this->receive_packet();

}

Intel8254xDriver Intel8254x;


extern "C"
{

    uint32_t i8254x_iobase_get(void)
    {
        return Intel8254x.iobase_get();
    }

    void i8254x_init(void)
    {
        return Intel8254x.init();
    }

    pci_device* i8254x_pci_info_get(void)
    {
        return Intel8254x.pci_info_get();
    }

    uint8_t* i8254x_mac_get(void)
    {
        return Intel8254x.mac_get();
    }

    uint32_t i8254x_receive_buffer_get(void)
    {
        return Intel8254x.receive_buffer_get();
    }

    uint32_t i8254x_transmit_buffer_get(void)
    {
        return Intel8254x.transmit_buffer_get();
    }

    void i8254x_interrupt_handler(void)
    {
        return Intel8254x.interrupt_handler();
    }

    void i8254x_packet_send(uint32_t address, uint16_t length)
    {
        return Intel8254x.send_packet(address, length);
    }

}