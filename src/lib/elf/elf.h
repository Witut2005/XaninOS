
#pragma once

#include <stdint.h>
#include <stddef.h>

enum ELF_FIELD
{
    X86_E_MACHINE = 0x3,
    ELF_HEADER_SIZE = 0x34,
    PT_LOAD = 0x1
};

enum ELF_PROPERTIES 
{
    EI_MAG_SIZE = 4,
    EI_PAD_SIZE = 7
};

struct ElfHeader 
{

    char ei_mag[EI_MAG_SIZE];
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abi_version;
    uint8_t ei_pad[EI_PAD_SIZE];

    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;

    size_t e_entry;
    size_t e_phoff;
    size_t e_shoff;

    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;

}__attribute__((packed));

typedef struct Elf32Header Elf32Header;

struct ElfProgramHeader
{
    uint32_t p_type;

    #if __x86_64__
    uint32_t p_flags;
    #endif

    size_t p_offset;
    size_t p_vaddr;
    size_t p_paddr;
    size_t p_filesz;
    size_t p_memsz;

    #if !defined(__x86_64__)
    uint32_t p_flags;
    #endif

    size_t p_align;
}__attribute__((packed));

typedef struct ElfProgramHeader ElfProgramHeader;

struct ElfSectionHeader
{
    uint32_t sh_name;
    uint32_t sh_type;

    size_t sh_flags;
    size_t sh_addr;
    size_t sh_offset;
    size_t sh_size;

    uint32_t sh_link;
    uint32_t sh_info;
    size_t sh_addralign;
    size_t sh_entsize;
};

typedef struct ElfSectionHeader ElfSectionHeader;