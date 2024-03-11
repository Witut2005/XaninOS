#include <sys/devices/nic/8254x.hpp>
#include <lib/libc/stdiox.h>
#include <lib/libcpp/hal.h>
#include <lib/libc/hal.h>
#include <sys/log/syslog.h>

#include <lib/libc/memory.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/net/network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <lib/libcpp/utility.h>
#include <limits.h>
#include <sys/interrupts/idt/idt.h>

#include <sys/pmmngr/alloc.h>
#include <sys/devices/vendor.h>

#define INTEL_8254X_DESCRIPTORS 256
#define reset() write(0x0, 0x80000000)

Intel8254xDriver* Intel8254x;

bool Intel8254xDriver::probe(const pci_device& device)
{
    return device.vendor_id == INTEL_VENDOR_ID && device.device_id == INTEL_8254X_VENDOR_ID;
}

NetworkDevice* Intel8254xDriver::create(const pci_device& device)
{

}

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

bool Intel8254xDriver::is_eeprom_present(void)
{
    this->write(nic::EERD, 0x1);

    bool eerprom_exists;

    for(int i = 0; i < 1000 && ! eerprom_exists; i++)
    {
            auto val = this->read(nic::EERD);
            if(val & 0x10)
                    eerprom_exists = true;
            else
                    eerprom_exists = false;
    }
    return eerprom_exists;


}

uint16_t Intel8254xDriver::eeprom_read(uint8_t address)
{

    /* enable reading from eeprom */
    this->write(nic::EECD, this->read(nic::EECD) | nic::EECD_REQ);
    while((this->read(nic::EECD) & nic::EECD_GNT) >> 7 == 0);

    if(!this->is_present)
        return USHRT_MAX;

    this->write(nic::EERD, (uint32_t)((address << 8) | 0x1));
    
    while(!(this->read(nic::EERD) & (1 << 4)));
    
    uint32_t ret = this->read(nic::EERD);
    this->write(nic::EECD, this->read(nic::EECD) ^ nic::EECD_REQ);

    ret = ret >> 16;


    return ret;

}

uint8_t* Intel8254xDriver::mac_get() 
{

    if(!this->is_present)
        return (uint8_t*)NULL;


    *(uint16_t*)&mac[0] = this->eeprom_read(0x0);
    *(uint16_t*)&mac[2] = this->eeprom_read(0x1);
    *(uint16_t*)&mac[4] = this->eeprom_read(0x2);

    return this->mac;
}


void Intel8254xDriver::receive_init(void)
{

    if(!this->is_present)
        return;

    this->receive_descriptors_buffer = (i8254xReceiveDescriptor*)kmalloc(SIZE_OF(i8254xReceiveDescriptor) * INTEL_8254X_DESCRIPTORS);
    const auto receive_buffer_size = 4096;
    this->receive_buffer = (uint8_t*)kmalloc(receive_buffer_size * INTEL_8254X_DESCRIPTORS);

    for(int i = 0; i < INTEL_8254X_DESCRIPTORS; i++)
    {
        this->receive_descriptors_buffer[i].address_low = (uint32_t)&this->receive_buffer[i * receive_buffer_size];
        this->receive_descriptors_buffer[i].address_high = 0x0;
        this->receive_descriptors_buffer[i].length = 0x0;
        this->receive_descriptors_buffer[i].checksum = 0x0;
        this->receive_descriptors_buffer[i].status = 0x0;
        this->receive_descriptors_buffer[i].errors = 0x0;
        this->receive_descriptors_buffer[i].special = 0x0;
    }

    /* receive buffer allocation */
    this->write(nic::RDBAL, (uint32_t)this->receive_descriptors_buffer);
    this->write(nic::RDBAH, 0x0);

    
    this->write(nic::RDLEN, INTEL_8254X_DESCRIPTORS * SIZE_OF(i8254xReceiveDescriptor));

    /* set head and tail to proper values */
    this->write(nic::RDH, 0x0);
    this->write(nic::RDT, INTEL_8254X_DESCRIPTORS - 1);

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

    if(!this->is_present)
        return;


    /* transmit buffer allocation */
    // auto AHA = (i8254xTransmitDescriptor*)malloc(SIZE_OF(i8254xTransmitDescriptor) * INTEL_8254X_DESCRIPTORS); //+ 16);

    auto transmit_descriptors_buffer_region = kmalloc(SIZE_OF(i8254xTransmitDescriptor) * INTEL_8254X_DESCRIPTORS); // kompilator zawsze ma racje, prawda? 
    this->transmit_descriptors_buffer = (i8254xTransmitDescriptor*)transmit_descriptors_buffer_region;

    // const auto TRANSMIT_BUFFER_SIZE = 4096;

    this->transmit_buffer = (uint8_t*)kmalloc(TRANSMIT_BUFFER_SIZE * INTEL_8254X_DESCRIPTORS);

    for(int i = 0; i < INTEL_8254X_DESCRIPTORS; i++)
    {
        this->transmit_descriptors_buffer[i].address_low = (uint32_t)&this->transmit_descriptors_buffer[i * TRANSMIT_BUFFER_SIZE];
        this->transmit_descriptors_buffer[i].address_high = 0x0;
        this->transmit_descriptors_buffer[i].length = 0x0;
        this->transmit_descriptors_buffer[i].cso = 0x0;
        this->transmit_descriptors_buffer[i].cmd = 0x0;
        this->transmit_descriptors_buffer[i].status = 0x0;
        this->transmit_descriptors_buffer[i].css = 0x0;
        this->transmit_descriptors_buffer[i].special = 0x0;
    }


    // while(1);

    this->write(nic::TDBAL, (uint32_t)transmit_descriptors_buffer_region);//this->transmit_descriptors_buffer);
    this->write(nic::TDBAH, 0x0);
    // this->transmit_descriptors_buffer = (i8254xTransmitDescriptor*)(this->read(nic::TDBAL));

    /* set head and tail to proper values */
    this->write(nic::TDH, 0x0);
    this->write(nic::TDT, INTEL_8254X_DESCRIPTORS);

    /* set buffer length */
    this->write(nic::TDLEN, INTEL_8254X_DESCRIPTORS * SIZE_OF(i8254xTransmitDescriptor));

    this->txd_current = 0x0;

    /* enable transmit packets */
    this->write(nic::TCTL, this->read(nic::TCTL) | nic::tctl::EN | nic::tctl::PSP);

    // this->write(nic::TCTL, 0x10 << 4); //coliision treshold
    // this->write(nic::TCTL, 0x40 << 12); //collision distance

    this->write(nic::TIPG, 0x0060200A);





}

void Intel8254xDriver::packet_send(uint8_t* address, uint16_t length)
{

    if(!is_present)
        return;

    // xprintf("a");

    this->transmit_descriptors_buffer[this->txd_current].address_low = (uint32_t)address;
    this->transmit_descriptors_buffer[this->txd_current].address_high = 0x0;
    this->transmit_descriptors_buffer[this->txd_current].cmd = nic::CMD::EOP | nic::CMD::IFCS | nic::CMD::RS | nic::CMD::RPS;// | nic::CMD::RPS;
    this->transmit_descriptors_buffer[this->txd_current].length = length;

    this->transmit_descriptors_buffer[this->txd_current].status = 0x0;
    this->transmit_descriptors_buffer[this->txd_current].css = 0x0;
    this->transmit_descriptors_buffer[this->txd_current].cso = 0x0;
    this->transmit_descriptors_buffer[this->txd_current].special = 0x0;


    auto txd_old = this->txd_current;
    this->write(nic::TDT,(this->txd_current + 1) % INTEL_8254X_DESCRIPTORS);
    this->txd_current = (this->txd_current + 1) % INTEL_8254X_DESCRIPTORS;

    while(!this->transmit_descriptors_buffer[txd_old].status);


}


void Intel8254xDriver::init()
{
    
    reset();

    for(int i = 0; i < 100; i++)
        io_wait();

    /* finding device */
    this->pci_selector = pci_find_device(INTEL_8254X, 0x100E, &pci_info);
    if(this->pci_selector == UINT32_MAX)
    {
        this->is_present = false;
        return;
    }

    else
        this->is_present = true;



    this->iobase = (uint8_t*)this->pci_info.base0;


    /* setting pci command register */
    uint16_t pci_command = pci_get_data16(pci_info.bus, pci_info.slot, pci_info.function, 0x4);
    pci_write_data16(pci_info.bus, pci_info.slot, pci_info.function, 0x4, pci_command | 0x7); 


    this->write(nic::EECD, this->read(nic::EECD) | nic::EECD_SK | nic::EECD_CS | nic::EECD_DI);
    this->mac_get();

    // /* general configuration (page 389) */
    this->write(nic::CTRL, this->read(nic::CTRL) | nic::ctrl::ASDE | nic::ctrl::SLU); // step 2
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::LRST)); // step 5
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::PHY_RST)); // step 6
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::ILOS)); // step 7 (can be bad)

    this->multicast_table_array_clear(); // clear multicast table
    this->multicast_table_array_clear(); // clear multicast table
    
    // /* disable VLANs */
    this->write(nic::CTRL, this->read(nic::CTRL) & (~nic::ctrl::VME));

    this->receive_init();
    this->transmit_init();

    this->last_packet = (uint8_t*)kcalloc(SIZE_OF(uint8_t) * XANIN_PMMNGR_BLOCK_SIZE);

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

uint32_t Intel8254xDriver::receive_descriptors_buffer_get(void)
{
    return this->read(nic::RDBAL);
}

uint32_t Intel8254xDriver::transmit_descriptors_buffer_get(void)
{
    return (uint32_t)this->transmit_descriptors_buffer;
    // return this->read(nic::TDBAL);
}

pci_device* Intel8254xDriver::pci_info_get() 
{
    return &this->pci_info;
}

uint32_t Intel8254xDriver::iobase_get() const
{
    return pci_info.base0;
}

uint16_t Intel8254xDriver::vendorid_get() const
{
    return this->pci_info.vendor_id;
}

uint8_t* Intel8254xDriver::packet_receive(void)
{

    if(!this->is_present)
        return (uint8_t*)NULL;

    this->rxd_current = this->read(nic::RDT) % INTEL_8254X_DESCRIPTORS;
    this->rxd_current = (this->rxd_current + 1) % INTEL_8254X_DESCRIPTORS;
    
    uint8_t* packet;
    uint16_t packet_lenght;
    
    while(!this->receive_descriptors_buffer[this->rxd_current].status & 0x1);
    io_wait();

    packet = (uint8_t*)this->receive_descriptors_buffer[rxd_current].address_low;
    packet_lenght = this->receive_descriptors_buffer[rxd_current].length;

    memcpy(this->last_packet, packet, packet_lenght);


    this->receive_descriptors_buffer[rxd_current].status = 0x0;
    this->write(nic::RDT, this->rxd_current);


    EthernetFrameInterface::receive(this->last_packet);

    return last_packet;

}


bool Intel8254xDriver::interrupt_handler(void) 
{
    uint16_t interrupt_status = this->read(nic::ICR);
    
    if(interrupt_status & 0x80)
    {
        this->packet_receive();
        return true;
    }

    return false;

}

uint32_t Intel8254xDriver::receive_buffer_get(void) const
{
    return (uint32_t)this->receive_buffer;
}

uint32_t Intel8254xDriver::transmit_buffer_get(void) const
{
    return (uint32_t)this->transmit_buffer;
}

bool Intel8254xDriver::is_device_present(void) const
{
    return this->is_present;
}

Intel8254xDriver::Intel8254xDriver() : name(NULL) {}

void Intel8254xDriver::name_set(const char* name)
{
    if(this->name != NULL)
        free(this->name);
    this->name = (char*)kcalloc(strlen(name));

    strcpy(this->name, name);
}

const char* Intel8254xDriver::name_get(void) const
{
    return this->name;
}

extern "C"
{

    uint32_t i8254x_receive_buffer_get(void)
    {
        return Intel8254x->receive_buffer_get();
    }

    uint32_t i8254x_transmit_buffer_get(void)
    {
        return Intel8254x->transmit_buffer_get();
    }

    uint32_t i8254x_iobase_get(void)
    {
        return Intel8254x->iobase_get();
    }


    pci_device* i8254x_pci_info_get(void)
    {
        return Intel8254x->pci_info_get();
    }

    uint8_t* i8254x_mac_get(void)
    {
        return Intel8254x->mac_get();
    }

    uint32_t i8254x_receive_descriptors_buffer_get(void)
    {
        return Intel8254x->receive_descriptors_buffer_get();
    }

    uint32_t i8254x_transmit_descriptors_buffer_get(void)
    {
        return Intel8254x->transmit_descriptors_buffer_get();
    }

    void i8254x_interrupt_handler(void)
    {
        Intel8254x->interrupt_handler();
        eoi_send();
    }

    uint8_t* i8254x_packet_receive(void)
    {
        return Intel8254x->packet_receive();
    }

    void i8254x_packet_send(uint8_t* address, uint16_t length)
    {
        NetworkDevice* dev = Intel8254x;
        dev->packet_send(address, length);
    }

    void i8254x_init(uint8_t vector)
    {
        Intel8254x = new Intel8254xDriver;
        NetworkDevice* NetDev = Intel8254x;

        Intel8254x->init();

        if(Intel8254x->is_device_present())
        {
            netapi_add_device(NetDev, "i8254x", i8254x_interrupt_handler_entry);
            INTERRUPT_REGISTER(vector, netapi_interrupt_handle_entry);
        }
    }

}