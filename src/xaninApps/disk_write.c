#pragma once

#include <devices/HARD_DISK/disk.h>

void disk_write_data(uint32_t sector, uint32_t address)
{
    disk_write(ATA_FIRST_BUS, ATA_MASTER, sector, address);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);
    app_exited = true;
}