
#pragma once

#include <lib/elf/elf.h>
#include "./disk.h"

// #define XANIN_OS_KERNEL_MEMORY_LOCATION (0x20200 + (15 * SECTOR_SIZE))
#define XANIN_OS_KERNEL_LOCATION (0x100000)

typedef uint8_t* address_t;

char* elf_section_header_string_table_address_get(ElfHeaderAuto* Header);
ElfSectionHeaderAuto* elf_load_given_section(ElfHeaderAuto* KernelHeader, const char* section_name);
void elf_load(ElfHeaderAuto* KernelHeader);
void elf_jump_to_entry_point(uint32_t entry_point, uint32_t init_array_address, uint32_t size_of_init_array_section, uint32_t size_of_init_array_entry);
