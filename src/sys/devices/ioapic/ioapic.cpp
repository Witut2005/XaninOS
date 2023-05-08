
#include <sys/log/syslog.h>

#include <sys/devices/ioapic/ioapic.hpp>

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
            printk("APIC BAD REGISTER SELECTED");
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
            printk("APIC BAD REGISTER SELECTED");
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


void InOutAdvancedProgrammableInterruptContoller::ioredtbl_configure(uint32_t lower, uint32_t upper)
{

    this->register_write(ioredtbl[ (lower - 0x20) & 0xFF].low, lower);        //interrupt settings
    this->register_write(ioredtbl[ (lower - 0x20) & 0xFF].high, upper << 24); //destinantion field

}

