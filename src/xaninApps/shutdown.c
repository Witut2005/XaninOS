
#include <libc/hal.h>
#include <libc/stdiox.h>
#include <devices/HARD_DISK/disk.h>
#include <xin_fs/xin.h>


void shutdown(void)
{
    xprintf("shutting down...\n");


    uint32_t k = 0;

    for(char* i = (char*)0x600; i < (char*)0x600 + SECTOR_SIZE; i++, k++)
        *i = enter_real_mode_buffer[k];
    
    k = 0;

    for(char* i = (char*)0x400; i < (char*)0x400 + SECTOR_SIZE; i++, k++)
        *i = shutdown_program_buffer[k];

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    for(int i = 0; i < 70; i++)
    {
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));
        disk_flush(ATA_FIRST_BUS, ATA_MASTER);
    }

    
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0, 1, (uint16_t*)0x7C00);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    //real_mode_enter(0x1000, 0x200); // <-- location in RAM of shutdown program

    // real_mode_enter_no_return(0x0000, 0x400); // <-- location in RAM of shutdown program
    // run16("/shutdown.bin");


    // asm volatile("mov esi, %0\n\t"
    //     "mov %1, 0xDEAD\n\t"
    //     "int 0x81"
    //     :
    //     : "r" (shutdown_file));



    real_mode_enter_no_return(0x0, 0x400);

    outwIO(0x604, 0x2000);  /* QEMU */
    outwIO(0xB004, 0x2000); /* BOCHS AND OLD QEMU */
    outwIO(0x4004, 0x3400); /* VIRTUALBOX */


    xprintf("goodbye :))\n\n");

    xprintf("Your computer status:\n\n");

    reg_dump();

    interrupt_disable();
    asm("hlt");

}
