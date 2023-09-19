
#pragma once

#include <stdint.h>
#include <stddef.h>

enum ELF_FIELD
{
    X86_E_MACHINE = 0x3,
    ELF_HEADER_SIZE = 0x34,
    PT_LOAD = 0x1
};

// enum ELF_PROPERTIES 
// {
// };

enum ELF_HEADER_PROPERTIES
{
    ELF_HEADER_MAGIC_DWORD = 0x464C457F,
    ELF_HEADER_32BIT_FORMAT = 1,
    ELF_HEADER_64BIT_FORMAT = 2,
    EI_MAG_SIZE = 4,
    EI_PAD_SIZE = 7
};

typedef uint8_t elfh_arch_t;

struct ElfHeader32 
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

    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;

    uint32_t e_flags;

    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;

}__attribute__((packed));
typedef struct ElfHeader32 ElfHeader32;

struct ElfHeader64 
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

    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;

    uint32_t e_flags;

    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;

}__attribute__((packed));
typedef struct Elf64Header Elf64Header;

#if __x86_64__
typedef ElfHeader64 ElfAutoHeader;
#else
typedef ElfHeader32 ElfAutoHeader;
#endif

////////////////////////////////////////////////////////////////

struct ElfProgramHeader32
{
    uint32_t p_type;

    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;

    uint32_t p_flags;

    uint32_t p_align;
}__attribute__((packed));
typedef struct Elf32ProgramHeader Elf32ProgramHeader;

struct ElfProgramHeader64
{
    uint32_t p_type;

    uint32_t p_flags;

    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;

    uint64_t p_align;
}__attribute__((packed));
typedef struct Elf64ProgramHeader Elf64ProgramHeader;

#if __x86_64__
typedef ElfHeader64 ElfProgramHeaderAuto;
#else
typedef ElfHeader32 ElfProgramHeaderAuto;
#endif

////////////////////////////////////////////////////////////////

struct ElfSectionHeader32
{
    uint32_t sh_name;
    uint32_t sh_type;

    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;

    uint32_t sh_link;
    uint32_t sh_info;

    uint32_t sh_addralign;
    uint32_t sh_entsize;
}__attribute__((packed));
typedef struct ElfSectionHeader32 ElfSectionHeader32;

struct ElfSectionHeader64
{
    uint32_t sh_name;
    uint32_t sh_type;

    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;

    uint32_t sh_link;
    uint32_t sh_info;

    uint64_t sh_addralign;
    uint64_t sh_entsize;
}__attribute__((packed));
typedef struct ElfSectionHeader64 ElfSectionHeader64;

#if __x86_64__
typedef ElfSectionHeader32 ElfSectionHeader32;
#else
typedef ElfSectionHeader64 ElfSectionHeader64;
#endif

static inline bool elf_header_magic_check(char* magic)
{
    return *(uint32_t*)magic == ELF_HEADER_MAGIC_DWORD;
}

static inline bool elf_header_check_if_32bit_format(uint8_t* ei_class)
{
    return *ei_class == ELF_HEADER_32BIT_FORMAT;
}

static inline bool elf_header_check_if_64bit_format(uint8_t* ei_class)
{
    return *ei_class == ELF_HEADER_64BIT_FORMAT;
}

static inline bool elf_header_if_match_given_architecture(uint8_t* e_machine, elfh_arch_t architecture)
{
    return *e_machine == architecture;
}

// 1B 00 00 00  sh_name
// 01 00 00 00  sh_type
// 02 00 00 00  sh_flags
// 74 80 04 08  sh_addr
// 74 00 00 00  sh_offset
// 03 00 00 00  sh_size
// 00 00 00 00  sh_link 
// 00 00 00 00  sh_info
// 01 00 00 00  sh_addralign 
// 00 00 00 00  sh_entsize 

