

#include <stdint.h>
#include <stddef.h>
#include <libcpp/cstdio.h>

enum IoApicInfoRegs
{
    ioapicid = 0x0,
    ioapicver = 0x1,
    ioapicarb = 0x2
};


enum IOREDTBL
{
    
    IOREDTBL0_LOW = 0x10,
    IOREDTBL0_HIGH = 0x11,
    IOREDTBL1_LOW = 0x12,
    IOREDTBL1_HIGH = 0x13,
    IOREDTBL2_LOW = 0x14,
    IOREDTBL2_HIGH = 0x15

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
        void ioredtbl_configure(uint8_t offset, uint32_t lower, uint32_t upper); 


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

    void ioapic_ioredtbl_configure(void)
    {
        IoApic.ioredtbl_configure(0x0,0x0,0x0);
    }


}

