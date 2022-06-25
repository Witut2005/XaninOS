
#include <stddef.h>
#include <stdint.h>
#include <devices/PCI/pci.h>

extern "C" int32_t pci_find_device(uint16_t);
extern "C" uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t bar_number);
extern "C" uint16_t pci_get_vendor_id(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function);

#define NE2000_PCI_CLASS 0x0200

namespace ne2000
{

    namespace page0 
    {

        enum Ne2000Register
        {
            command           = 0x0,
            cldma0            = 0x1,
            cldma1            = 0x2,
            boundary          = 0x3,
            transmit_status   = 0x4,
            collsiion_counter = 0x5,
            fifo              = 0x6,
            isr               = 0x7,
            crda0             = 0x8,
            crda1             = 0x9,
            id0               = 0xa,
            id1               = 0xb,
            receive_status    = 0xc,
            cntr0             = 0xd,
            cntr1             = 0xe,
            cntr2             = 0xf
        };
        
    };


}

class Ne2000Manager
{
    public:
    uint32_t iobase;
    uint32_t pci_selector;
    pci_device pci_info;

    Ne2000Manager();
    uint32_t mac_get();
    uint32_t iobase_get();
    uint16_t vendorid_get();




}Ne2000;

extern "C"
{

    uint32_t ne2000_iobase_get(void)
    {
        return Ne2000.iobase_get();
    }

    uint16_t ne2000_vendorid_get(void)
    {
        return Ne2000.vendorid_get();
    }

}