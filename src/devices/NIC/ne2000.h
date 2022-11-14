#include <devices/PCI/pci.h>

#define NE2000_DEVICE_ID 0x8029
#define NE2000_VENDOR_ID 0x10EC
#define NE2000_CLASS_ID 0x200

#ifdef __cplusplus

class Ne2000
{
    void init(void);
    char* mac_get(void);   
    pci_device device_info;
    uint8_t mac[6];
    uint32_t pci_selector;
};

namespace ne2000{
    namespace page0
    {
#endif

        enum NE2000_REGISTERS_PAGE0
        {
            //read
            COMMAND = 0,
            CLDA0 = 1,
            CLDA1 = 2,
            BNRY = 3,
            TSR = 4,
            NCR = 5,
            FIFO = 6,
            ISR = 7,
            CRDA0 = 8,
            CRDA = 9,
            RSR = 12,

            //write
            PTART = 1,
            PSTOP = 2,
            TPSR = 4,
            TBCR0 = 5,
            TBCR1 = 6,
            RSAR0 = 8,
            RSAR1 = 9,
            RBCR0 = 10,
            RBCR1 = 11,
            RCR = 12,
            TCR = 13,
            DCR = 14,
            IMR = 15


        };

#ifdef __cplusplus
    }


}
#endif