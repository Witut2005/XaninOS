
#include <stddef.h>
#include <stdint.h>
#include <devices/PCI/pci.h>

extern "C" int32_t pci_find_device(uint16_t, pci_device*);
extern "C" uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t bar_number);
extern "C" uint16_t pci_get_vendor_id(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function);
extern "C" uint16_t pci_get_data16(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset);

#define INTEL_8254X 0x0200

namespace nic
{

    enum NIC_DATA
    {
        IOADDR = 0x0, 
        IODATA = 0x1,
        EECD_SK = 0x1,
        EECD_CS = 0x2,
        EECD_DI = 0x4,
        EECD_REQ = 0x1 << 6,
        EECD_GNT = 0x1 << 7
    };

    enum NIC_REGISTERS
    {

        CTRL = 0x0, 
        STATUS = 0x8,
        EECD = 0x10,
        EERD = 0x14,
        FLA = 0x1C,
        CTRL_EXT = 0x18,
        MDIC = 0x20,


    };



}

class Intel8254x
{
    public:
    uint8_t* iobase;

    uint32_t pci_selector;
    pci_device pci_info;

    void write(uint32_t reg, uint32_t value);
    uint32_t read(uint32_t reg);

    uint32_t mac_get();
    uint32_t iobase_get();
    uint16_t vendorid_get();
    void init();
    pci_device* pci_info_get();
    uint16_t eeprom_read(uint8_t address);





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
