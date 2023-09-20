
#pragma once

#include <lib/elf/elf.h>

char* elf_section_header_string_table_address_get(ElfAutoHeader* Header);
bool elf_load_given_section(ElfAutoHeader* Header, const char* section_name);
void elf_load(void);