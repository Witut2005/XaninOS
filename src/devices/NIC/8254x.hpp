
#include <stddef.h>
#include <stdint.h>
#include <devices/PCI/pci.h>

extern "C" int32_t pci_find_device(uint16_t, pci_device*);
extern "C" uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t bar_number);
extern "C" uint16_t pci_get_vendor_id(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function);
extern "C" uint16_t pci_get_data16(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset);
extern "C" uint16_t pci_write_data16(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset, const uint16_t value);

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
        EECD_GNT = 0x1 << 7,
        RCTL_EN = 0x2
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
        FCAL = 0x28,
        FCAH = 0x2C,
        FCT = 0x30,
        VET = 0x38,
        FCTTV = 0x170, 
        TXCW = 0x178,
        RXCW = 0x180,
        LECTL = 0xE00,
        PBA = 0x1000,
        ICR = 0xC0,
        ITR = 0xC4,
        ICS = 0xC8,
        IMS = 0xD0,
        IMC = 0xD8,
        RCTL = 0x100


    };



}

class Intel8254xDriver
{
    public:
    uint8_t* iobase;
    uint32_t pci_selector;
    
    pci_device pci_info;
    uint8_t mac[6];
    
    //---------------------------------

    void write(uint32_t reg, uint32_t value);
    uint32_t read(uint32_t reg);
    uint8_t* mac_get();
    uint32_t iobase_get();
    uint16_t vendorid_get();
    void init();
    pci_device* pci_info_get();
    uint16_t eeprom_read(uint8_t address);





}Intel8254x;

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

}
