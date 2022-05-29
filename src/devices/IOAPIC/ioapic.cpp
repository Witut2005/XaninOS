
#include <devices/IOAPIC/ioapic.hpp>

void InOutAdvancedProgrammableInterruptContoller::base_address_set(uint32_t base)
{
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
            *ioregsel = offset;
            break;
        case 0x10 ... 0x3F:
            *ioregsel = offset;
            break;
        default:
            xprintf("%zIOAPIC BAD REGISTER SELECTED\n", stderr);
            return 0xFFFFFFFF;
    }

    return *iowin;

}