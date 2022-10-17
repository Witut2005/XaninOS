
#pragma once

 
#include <stdint.h>
#include <libc/hal.h>
#include <devices/HARD_DISK/disk.h>



int reboot(void)
{

uint16_t idt_16[3] = {0x0, 0x0, 0x0};


    for(int i = 0; i < 5; i++)
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12 + i, 1, (uint16_t*)(0x800 + (i * SECTOR_SIZE)));

    for(int i = 0; i < 10; i++)
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));



    asm("lidt %0" :: "m"(idt_16));
    
    uint8_t s = 12;
    s = s / 0;

    return XANIN_OK;

}
