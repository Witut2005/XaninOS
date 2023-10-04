
#include <stdint.h>
#include "./boot_libs/disk.h"
#include "./boot_libs/elf.h"

void _start(void)
{
    init_disk(ATA_FIRST_BUS, ATA_MASTER);    
        
    for(int i = 0; i < 1800; i++) // Weird reboot, shutdown bug (probably cpu want to make fun of me (ivt))
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0xA9 + 14 + i, 1, (uint16_t*)(0x20200 + ((15 + i)* SECTOR_SIZE)));

    elf_load((ElfHeaderAuto*)XANIN_OS_KERNEL_MEMORY_LOCATION);

    ElfSectionHeaderAuto* KernelInitArraySection = elf_load_given_section((ElfHeaderAuto*)XANIN_OS_KERNEL_MEMORY_LOCATION, ".init_array");
    elf_jump_to_entry_point(((ElfHeaderAuto*)XANIN_OS_KERNEL_MEMORY_LOCATION)->e_entry, KernelInitArraySection->sh_addr, KernelInitArraySection->sh_size, KernelInitArraySection->sh_entsize);
}