
#include <lib/libc/memory.h>
#include "./elf.h"

ElfSectionHeader32* elf_section_table_offset_get(ElfHeaderAuto* Header)
{
    return (ElfSectionHeader32*)Header->e_shoff;
}
