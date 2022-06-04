

#include <stdint.h>
#include <stddef.h>
#include <libcpp/cstdio.h>

enum IoApicInfoRegs
{
    ioapicid = 0x0,
    ioapicver = 0x1,
    ioapicarb = 0x2
};

struct InputOutputRedirectionTable 
{
    uint8_t low;
    uint8_t high;
};

InputOutputRedirectionTable ioredtbl[24] = 
{

    {0x10, 0x11},   //IOREDTBL0
    {0x12, 0x13},
    {0x14, 0x15},
    {0x16, 0x17},
    {0x18, 0x19},
    {0x1A, 0x1B},
    {0x1C, 0x1D},
    {0x1E, 0x1F},
    {0x20, 0x21},
    {0x22, 0x23},
    {0x24, 0x25},
    {0x26, 0x27},
    {0x28, 0x29},
    {0x2A, 0x2B},
    {0x2C, 0x2D},
    {0x2E, 0x2F},
    {0x30, 0x31},
    {0x32, 0x33},
    {0x34, 0x35},
    {0x36, 0x37},
    {0x38, 0x39},
    {0x3A, 0x3B},
    {0x3C, 0x3D},
    {0x3E, 0x3F}    //IOREDTBL23

}; 


class InOutAdvancedProgrammableInterruptContoller
{

    enum 
    {
        
    };

    private:
        uint32_t* base;
        uint32_t* ioregsel;
        uint32_t* iowin;
    
 
    public:
    
        void register_write(uint8_t offset, uint32_t value);
        uint32_t register_read(uint8_t offset);
        void base_address_set(uint32_t base);
        uint32_t id_get();
        uint32_t version_get();        
        void ioredtbl_configure(uint32_t lower, uint32_t upper); 


}IoApic;


extern "C"
{
    void ioapic_init(uint32_t base)
    {
        IoApic.base_address_set(base); 
    }

    void ioapic_write(uint8_t offset, uint32_t value)
    {
        IoApic.register_write(offset, value);
    }

    uint32_t ioapic_read(uint8_t offset)
    {
        return IoApic.register_read(offset);
    }

    uint32_t ioapic_id_get(void)
    {
        return IoApic.id_get();        
    }

    void ioapic_ioredtbl_configure(uint32_t lower, uint32_t upper)
    {
        IoApic.ioredtbl_configure(lower, upper);
    }


}

