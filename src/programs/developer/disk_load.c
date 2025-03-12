
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <sys/call/xanin_sys/handler/xanin_sys.h>
#include <sys/devices/hda/disk.h>

// TERMINAL_APP

int disk_load(char* sector_str, char* how_many_sectors_str, char* address_str)
{
    uint32_t sector = strtoi(sector_str, STRTOI_HEXADECIMAL);
    uint8_t how_many_sectors = strtoi(how_many_sectors_str, STRTOI_HEXADECIMAL);
    uint32_t address = strtoi(address_str, STRTOI_HEXADECIMAL);

    xprintf("reading 0x%x\n", sector);
    xprintf("address 0x%x\n", address);

    disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, sector, how_many_sectors, (uint16_t*)address);
    // disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, 0, 1, (uint16_t *)0);
    // disk_read(sector, how_many_sectors, (uint16_t *)address);
    return XANIN_OK;
}
