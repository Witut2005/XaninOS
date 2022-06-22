
#include <libc/hal.h>
#include <libc/stdiox.h>
#include <xin_fs/xanin_fs_saver.c>
#include <xaninApps/execute_addr.c>
#include <devices/HARD_DISK/disk.h>

uint8_t enter_real_mode_buffer[512];
uint8_t shutdown_program_buffer[512];


void shutdown(void)
{
    xprintf("shutting down...\n");


    uint32_t k = 0;

    for(char* i = (char*)0x600; i < (char*)0x600 + 0x200; i++, k++)
        *i = enter_real_mode_buffer[k];
    
    k = 0;

    for(char* i = (char*)0x10200; i < (char*)0x10200 + 0x200; i++, k++)
        *i = shutdown_program_buffer[k];

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    for(int i = 0; i < 70; i++)
    {
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));
        disk_flush(ATA_FIRST_BUS, ATA_MASTER);
    }

    //real_mode_enter(0x1000, 0x200); // <-- location in RAM of shutdown program

    real_mode_enter(0x0000, 0x400); // <-- location in RAM of shutdown program


    outwIO(0x604, 0x2000);  /* QEMU */
    outwIO(0xB004, 0x2000); /* BOCHS AND OLD QEMU */
    outwIO(0x4004, 0x3400); /* VIRTUALBOX */

    screen_clear();

    xprintf("goodbye :))\n\n");

    xprintf("Your computer status:\n\n");

    reg_dump();

    interrupt_disable();
    asm("hlt");

}
