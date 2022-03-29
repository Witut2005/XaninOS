#pragma once

#include <devices/HARD_DISK/disk.h>

void disk_write_data(uint32_t sector, uint8_t how_many_sectors, uint32_t address)
{
    disk_write(ATA_FIRST_BUS, ATA_MASTER, sector, how_many_sectors, address);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);
}