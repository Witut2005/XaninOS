
#pragma once

#include <lib/elf/elf.h>
#include "./disk.h"

#define XANIN_OS_KERNEL_MEMORY_LOCATION (0x20200 + (15 * SECTOR_SIZE))

char* elf_section_header_string_table_address_get(ElfHeaderAuto* Header);
bool elf_load_given_section(ElfHeaderAuto* Header, const char* section_name);
void elf_load(void);