#pragma once

#include <devices/HARD_DISK/disk.h>

void disk_load(uint32_t sector, uint8_t how_many_sectors, uint32_t address)
{
    disk_read(ATA_FIRST_BUS, ATA_MASTER, sector, how_many_sectors, address);
}