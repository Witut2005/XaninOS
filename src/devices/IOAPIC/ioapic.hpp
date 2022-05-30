

#include <stdint.h>
#include <stddef.h>
#include <libcpp/cstdio.h>

enum IoApicInfoRegs
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

    union RedirectionEntry
    {
        struct 
        {
            uint64_t vector          : 8;
            uint64_t delivery_mode   : 3;
            uint64_t destination_mode: 1;
            uint64_t delivery_status : 1;
            uint64_t pin_polarity    : 1;
            uint64_t remote_irr      : 1;
            uint64_t trigger_mode    : 1;
            uint64_t mask            : 1;
            uint64_t reserved        : 39;
            uint64_t destination     : 8;
        };

        struct 
        {
            uint32_t lower32;
            uint32_t upper32;
        };

    };
    
 
    public:
        

        void register_write(uint8_t offset, uint32_t value);
        uint32_t register_read(uint8_t offset);
        void base_address_set(uint32_t base);
        uint32_t id_get();
        uint32_t version_get();        

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

