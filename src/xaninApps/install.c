
#pragma once

#include <devices/HARD_DISK/disk.h>

void install(void)
{

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0, 1, 0x7c00);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0xD + 1, 0xA0, 0x20000);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x3, 1, 0x600);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);
}