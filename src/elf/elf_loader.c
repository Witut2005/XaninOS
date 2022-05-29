
#pragma once

#include <elf/elf_loader.h>
#include <stddef.h>


enum ELF_FIELD
{
    X86_E_MACHINE = 0x3,
    ELF_HEADER_SIZE = 0x34,
    PT_LOAD = 0x1
};

bool elf_check_magic(xin_entry* file)
{
    char tmp[4];
    read(file, tmp, 4);
    return strncmp(tmp + 1, "ELF", 3) && tmp[0] == 0x7F;
}

bool elf_check_arch(xin_entry* file)
{
    uint16_t tmp;
    fseek(file, 0x12);
    read(file, &tmp, 2);
    return tmp == X86_E_MACHINE;
}

void elf_load(xin_entry* file)
{
    uint8_t* data = file->starting_sector * SECTOR_SIZE;
    uint8_t* write_to_memory;
    uint8_t* read_from_file;
    uint32_t file_base = file->starting_sector * SECTOR_SIZE;

    uint16_t phnum = *(uint16_t*)((uint8_t*)data + 0x2C);

    uint32_t p_offset;      //offset in file image
    uint32_t p_vaddr;       //virtual address of the segment in memory
    uint32_t p_filesz;      //size in bytes of segment in file image
    uint32_t p_memsz;       //size in bytes of segment in memory

    data += ELF_HEADER_SIZE;

    while(phnum)
    {
        if(*data != PT_LOAD)
        {   
            data += 0x20;
            continue;
        }

        p_offset = *(uint32_t*)((uint8_t*)data + 0x4) + file_base;
        p_vaddr  = *(uint32_t*)((uint8_t*)data + 0x8);
        p_filesz = *(uint32_t*)((uint8_t*)data + 0x10);
        p_memsz  = *(uint32_t*)((uint8_t*)data + 0x14);

        read_from_file = p_offset;
        write_to_memory = p_vaddr;

        for(int i = 0; i < p_filesz; i++, write_to_memory++, read_from_file++)
        {
            *write_to_memory = *read_from_file;
        }


        phnum--;


    }

}