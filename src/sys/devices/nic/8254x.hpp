
#include <stddef.h>
#include <stdint.h>
#include <sys/devices/pci/pci.h>
#include <sys/net/netapi/network_device.h>

// extern "C" int32_t pci_find_device(uint16_t, uint16_t, pci_device*);
extern "C" uint32_t pci_get_bar(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t bar_number);
extern "C" uint16_t pci_get_vendor_id(const uint8_t bus, const uint8_t slot, 
                        const uint8_t function);
extern "C" uint16_t pci_write_data16(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset, const uint16_t value);
extern "C" bool i8254x_interrupt_handler_entry(void);

#define INTEL_8254X 0x0200
#define INTEL_8254X_VENDOR_ID 0x100E
#define TRANSMIT_BUFFER_SIZE XANIN_PMMNGR_BLOCK_SIZE

namespace nic
{

    enum NIC_DATA
    {
        IOADDR   = 0x0, 
        IODATA   = 0x1,
        EECD_SK  = 0x1,
        EECD_CS  = 0x2,
        EECD_DI  = 0x4,
        EECD_REQ = 0x1 << 6,
        EECD_GNT = 0x1 << 7,
        RCTL_EN  = 0x2
    };

    enum ctrl 
    {
        FD    = 1 << 0,
        RESV0 = 1 << 1,
        LRST  = 1 << 3,
        RESV1 = 1 << 4,
        ASDE  = 1 << 5,
        SLU = 1 << 6,
        ILOS = 1 << 7,
        SPEED = 1 << 8,
        RESV2 = 1 << 10,
        FRCSPD = 1 << 11,
        VME = 1 << 30,        
        PHY_RST = 1 << 31


    };

    enum ims 
    {

        RXO = 0x1 << 6,
        RXT = 1 << 7,
        RXDMT = 1 << 4,
        RXSEQ = 1 << 3,
        LSC = 1 << 2

    };

    enum rctl 
    {
        LBM = 1 << 6,
        BAM = 1 << 15,
        BSIZE = 1 << 16,
        BSEX = 1 << 26,


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
        RCTL = 0x100,
        MTA = 0x5200,
        RDBAL = 0x2800,
        RDBAH = 0x2804,
        RDLEN = 0x2808,
        RDH = 0x2810,
        RDT = 0x2818,
        RDTR = 0x2820,
        TDBAL = 0x3800,
        TDBAH = 0x3804,
        TDLEN = 0x3808,
        TDH = 0x3810,
        TDT = 0x3818,
        TCTL = 0x400,
        TIPG = 0x410


    };

    enum tctl
    {
        EN = 1 << 1,
        PSP = 1 << 3,
        CT = 1 << 4,
        COLD = 1 << 12,
        NRTU = 1 << 25
    };

    enum CMD
    {
        EOP = 1 << 0,
        IFCS = 1 << 1,
        IC = 1 << 2,
        RS = 1 << 3,
        RPS = 1 << 4,
        RSV = 1 << 4,
        DEXT = 1 << 5, 
        VLE = 1 << 6,
        IDE = 1 << 6
    };


}

struct i8254xReceiveDescriptor
{

    uint32_t address_low;
    uint32_t address_high;

    uint16_t length;
    uint16_t checksum;
    uint8_t status;
    uint8_t errors;
    uint16_t special;

}__attribute__((packed));


struct i8254xTransmitDescriptor
{

    uint32_t address_low;
    uint32_t address_high;

    uint16_t length;
    uint8_t cso;
    uint8_t cmd;
    uint8_t status; //status and rsv (dext legacy mode)
    uint8_t css;
    uint16_t special;

}__attribute__((packed));

class Intel8254xDriver : public NetworkDevice
{


    private:
    bool is_present;
    uint8_t* iobase;
    uint32_t pci_selector;
    pci_device pci_info;
    uint8_t mac[6];

    i8254xReceiveDescriptor* receive_descriptors_buffer;
    i8254xTransmitDescriptor* transmit_descriptors_buffer;
    
    uint8_t* receive_buffer;
    uint8_t* transmit_buffer;

    uint32_t txd_current;
    uint32_t rxd_current;
    uint8_t* last_packet;
    char* name;
    
    //---------------------------------

    public:
    Intel8254xDriver();
    void write(uint32_t reg, uint32_t value);
    uint32_t read(uint32_t reg);
    virtual uint8_t* mac_get(void);
    uint32_t iobase_get(void) const;
    uint16_t vendorid_get(void) const;
    virtual pci_device* pci_info_get(void);
    uint16_t eeprom_read(uint8_t address);
    bool is_eeprom_present(void);
    void multicast_table_array_clear(void);
    uint32_t receive_descriptors_buffer_get(void);
    uint32_t transmit_descriptors_buffer_get(void);
    uint32_t receive_buffer_get(void) const;
    uint32_t transmit_buffer_get(void) const;
    virtual bool interrupt_handler(void);
    void receive_init(void);
    void transmit_init(void);
    void init(void);
    bool is_device_present(void) const;
    virtual void name_set(const char* name);
    virtual const char* name_get(void) const;
    virtual uint8_t* packet_receive(void);
    virtual void packet_send(uint8_t* address_low, uint16_t length);

};
