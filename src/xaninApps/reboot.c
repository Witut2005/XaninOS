
#pragma once

 
#include <stdint.h>
#include <libc/hal.h>
#include <devices/HARD_DISK/disk.h>



void reboot(void)
{

uint16_t idt_16[3] = {0x0, 0x0, 0x0};

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);

    for(int i = 0; i < 70; i++)
    {
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));
    }

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x0, 1, (uint16_t*)0x7c00);
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x0, 1, (uint16_t*)0x7c00);
    
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x82, 1, (uint16_t*)kernel_load_backup);

    asm("lidt %0" :: "m"(idt_16));
    
    uint8_t s = 12;
    s = s / 0;

    

}
