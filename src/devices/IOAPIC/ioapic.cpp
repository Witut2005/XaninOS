
#include <devices/IOAPIC/ioapic.hpp>

void InOutAdvancedProgrammableInterruptContoller::base_address_set(uint32_t base)
{
    this->base = (uint32_t*)base;
    ioregsel = (uint32_t*)base;
    iowin = (uint32_t*)((uint8_t*)base + 0x10);
}

void InOutAdvancedProgrammableInterruptContoller::register_write(uint8_t offset, uint32_t value)
{
    
    switch(offset)
    {
        case 0 ... 2:
            *ioregsel = offset;
            break;
        case 0x10 ... 0x3F:
            *ioregsel = offset;
            break;
        default:
            xprintf("%zAPIO BAD REGISTER SELECTED\n", stderr);
            return;
    }

    *iowin = value;

}


uint32_t InOutAdvancedProgrammableInterruptContoller::register_read(uint8_t offset)
{
    switch(offset)
    {
        case 0 ... 2:
        {
            *ioregsel = offset;
            break;
        }

        case 0x10 ... 0x3F:
        {
            *ioregsel = offset;
            break;
        }
        default:
        {
            xprintf("%zIOAPIC BAD REGISTER SELECTED\n", stderr);
            return 0xFFFFFFFF;
        }
    }

    return *iowin;

}


uint32_t InOutAdvancedProgrammableInterruptContoller::id_get()
{   
    return this->register_read(ioapicid);
}

uint32_t InOutAdvancedProgrammableInterruptContoller::version_get()
{
    return this->register_read(ioapicver);
}


void InOutAdvancedProgrammableInterruptContoller::ioredtbl_configure(uint8_t offset, uint32_t lower, uint32_t upper)
{

    this->register_write(IOREDTBL1_LOW, (0x21 << 0) | (0x0 << 8) | (0 << 11) | 
            (0 << 13) | ( 0 << 15) | (0 << 16));
    this->register_write(IOREDTBL1_HIGH, this->id_get() << 24);

    this->register_write(IOREDTBL2_LOW, (0x22 << 0) | (0x0 << 8) | (0 << 11) | 
            (0 << 13) | ( 0 << 15) | (0 << 16));
    this->register_write(IOREDTBL2_HIGH, this->id_get() << 24);
}

