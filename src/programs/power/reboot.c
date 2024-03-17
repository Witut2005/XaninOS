
#include <lib/libc/stdiox.h>
#include <lib/libc/hal.h>
#include <sys/devices/keyboard/keyboard.h>

int reboot(void)
{
    interrupt_disable();
    *(uint16_t*)VGA_TEXT_MEMORY = 0x4141;
    keyboard_cpu_reset();
    // xprintf("nicho");

    // screen_clear();
    uint16_t idt_16[3] = { 0x0, 0x1234, 0x4567 };

    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(0x800));
    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 20, (uint16_t*)(0x1800));

    asm("lidt %0" :: "m"(idt_16));

    uint8_t s = 12;
    s = s / 0;

    return XANIN_OK;

}
