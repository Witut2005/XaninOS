
#include <lib/libc/stdiox.h>

int reboot(void)
{

    // xprintf("nicho");
    *(uint16_t*)VGA_TEXT_MEMORY = 0x4141;

    // screen_clear();
    uint16_t idt_16[3] = {0x0, 0x1234, 0x4567};

    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(0x800));
    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 20, (uint16_t*)(0x1800));

    asm("lidt %0" :: "m"(idt_16));
    
    uint8_t s = 12;
    s = s / 0;

    return XANIN_OK;

}
