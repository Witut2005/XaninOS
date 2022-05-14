
#pragma once

 
#include <stdint.h>
#include <lib/hal.h>
#include <devices/HARD_DISK/disk.h>



void reboot(void)
{

uint16_t idt_16[3] = {0x0, 0x0, 0x0};

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 40, (uint16_t*)0x1800);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    asm("lidt %0" :: "m"(idt_16));
    
    uint8_t s = 12;
    s = s / 0;

    real_mode_enter(0x1000, 0x400);



}
