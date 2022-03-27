#pragma once

#include <devices/HARD_DISK/disk.h>

void disk_load(uint32_t sector, uint32_t address)
{
    disk_read(ATA_FIRST_BUS, ATA_MASTER, sector, 0, address);
    app_exited = true;
}