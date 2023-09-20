
#include <stdint.h>
#include "./boot_libs/disk.h"

void _start(void)
{
    init_disk(ATA_FIRST_BUS, ATA_MASTER);    
        
    for(int i = 0; i < 1600; i++) // Weird reboot, shutdown bug (probably cpu want to make fun of me (ivt))
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0xA9 + 14 + i, 1, (uint16_t*)(0x20200 + ((15 + i)* SECTOR_SIZE)));

    elf_load();
}