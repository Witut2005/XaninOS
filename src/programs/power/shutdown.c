
#include <lib/libc/hal.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/hda/disk.h>
#include <fs/xin.h>

int shutdown(void)
{

    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(0x800));
    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 10, (uint16_t*)(0x1800));

    outwIO(0x604, 0x2000);  /* QEMU */
    outwIO(0xB004, 0x2000); /* BOCHS AND OLD QEMU */
    outwIO(0x4004, 0x3400); /* VIRTUALBOX */
    real_mode_enter_no_return(0x0, 0x400);

    xprintf("goodbye :))\n\n");

    interrupt_disable();
    asm("hlt");
    return XANIN_OK;
}
