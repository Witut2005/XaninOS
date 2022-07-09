
#include <stddef.h>
#include <stdint.h>
#include <devices/PCI/pci.h>

extern "C" int32_t pci_find_device(uint16_t, pci_device*);
extern "C" uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t bar_number);
extern "C" uint16_t pci_get_vendor_id(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function);
extern "C" uint16_t pci_get_data16(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset);

#define NE2000_PCI_CLASS 0x0200

namespace nic
{

    enum NIC_DATA
    {

        IOADDR = 0x0, 
        IODATA = 0x1

    };


}

class Intel8254x
{
    public:
    uint32_t* iobase;
    uint32_t* ioaddr;
    uint32_t* iodata;
    uint32_t pci_selector;
    pci_device pci_info;

    void write(uint32_t ioaddr, uint32_t value);
    uint32_t read(void);

    uint32_t mac_get();
    uint32_t iobase_get();
    uint16_t vendorid_get();
    void init();
    pci_device* pci_info_get();





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

    void ne2000_init(void)
    {
        return Ne2000.init();
    }

    pci_device* ne2000_pci_info_get(void)
    {
        return Ne2000.pci_info_get();
    }

}
