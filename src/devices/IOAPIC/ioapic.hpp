

#include <stdint.h>
#include <stddef.h>
#include <libcpp/cstdio.h>

enum class IoApicInfoRegs
{
    ioapicid = 0x0,
    ioapicver = 0x1,
    ioapicarb = 0x2
};

class InOutAdvancedProgrammableInterruptContoller
{


    private:
        uint32_t* ioregsel;
        uint32_t* iowin;

    public:
        

        void register_write(uint8_t offset, uint32_t value);
        uint32_t register_read(uint8_t offset);
        void base_address_set(uint32_t base);
        
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

}

