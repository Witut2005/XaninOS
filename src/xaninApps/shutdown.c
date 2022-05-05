
#include <lib/hal.h>
#include <lib/stdiox.h>
#include <xin_fs/xanin_fs_saver.c>
#include <xaninApps/execute_addr.c>

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

    real_mode_enter(0x1000, 0x200); // <-- location in RAM of shutdown program


    outwIO(0x604, 0x2000);  /* QEMU */
    outwIO(0xB004, 0x2000); /* BOCHS AND OLD QEMU */
    outwIO(0x4004, 0x3400); /* VIRTUALBOX */

    screen_clear();

    xprintf("goodbye :))\n\n");

    xprintf("Your computer status:\n\n");

    reg_dump();

    asm("cli");
    asm("hlt");

}
