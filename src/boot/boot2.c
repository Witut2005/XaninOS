
#include <stdint.h>
#include "./boot_libs/disk.h"
#include "./boot_libs/elf.h"

// be carefull about this
//https://wiki.osdev.org/Memory_Map_(x86)

void _start(void)
{

#if XANIN_OS_KERNEL_LOCATION < 0x100000
#error "KERNEL must be loaded after Motherboard BIOS memory space"
#endif
    disk_init(ATA_FIRST_BUS, ATA_MASTER);

    for (int i = 0; i < 2500; i++) {
        disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, 0xA9 + 14 + i, 1, (uint16_t*)(XANIN_OS_KERNEL_LOCATION + (i * SECTOR_SIZE)));
    }

    elf_load((ElfHeaderAuto*)XANIN_OS_KERNEL_LOCATION);
    ElfSectionHeaderAuto* KernelInitArraySection = elf_load_given_section((ElfHeaderAuto*)XANIN_OS_KERNEL_LOCATION, ".init_array");

    elf_jump_to_entry_point(((ElfHeaderAuto*)XANIN_OS_KERNEL_LOCATION)->e_entry, KernelInitArraySection->sh_addr, KernelInitArraySection->sh_size, KernelInitArraySection->sh_entsize);

}