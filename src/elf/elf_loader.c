
#pragma once

#include <elf/elf_loader.h>
#include <stddef.h>


enum ELF_FIELD
{
    X86_E_MACHINE = 0x3,
    ELF_HEADER_SIZE = 0x34,
    PT_LOAD = 0x1
};

bool elf_check_magic(uint8_t* data)
{
    return strncmp(data + 1, "ELF", 3);// && data[0] == 0x7F;
}

bool elf_check_arch(uint8_t* file)
{
    uint16_t tmp;

    tmp = file[0];
    tmp += file[1] << 8;

    return tmp == X86_E_MACHINE;

}

void elf_load(xin_entry* file)
{
    uint8_t* data = (uint8_t*)calloc(file->entry_size * SECTOR_SIZE);
    fread(file, data, file->entry_size * SECTOR_SIZE);

    xprintf("file size: %d\n", file->entry_size);

    uint8_t* write_to_memory;
    uint8_t* read_from_file;
    uint32_t file_base = data;

    uint16_t phnum = *(uint16_t*)((uint8_t*)data + 0x2C);

    uint32_t p_offset;      //offset in file image
    uint32_t p_vaddr;       //virtual address of the segment in memory
    uint32_t p_filesz;      //size in bytes of segment in file image
    uint32_t p_memsz;       //size in bytes of segment in memory
    uint32_t entry_point = *(uint32_t*)((uint8_t*)data + 0x18);

    if(!elf_check_magic(data))
    {
        xprintf("%zBAD MAGIC\n", stderr);
        while(inputg().scan_code != ENTER);
        return;
    }

    if(!elf_check_arch(data + 0x12))
    {
        xprintf("%zBAD ARCH\n", stderr);
        while(inputg().scan_code != ENTER);
        return;
    }

    data += ELF_HEADER_SIZE;
    uint32_t load_sum = 0;

    screen_clear();

    while(phnum)
    {
        
        if(*(uint32_t*)data == PT_LOAD)
        {

            load_sum++;

            p_offset = *(uint32_t*)((uint8_t*)data + 0x4) + file_base;
            p_vaddr  = *(uint32_t*)((uint8_t*)data + 0x8);
            p_filesz = *(uint32_t*)((uint8_t*)data + 0x10);
            p_memsz  = *(uint32_t*)((uint8_t*)data + 0x14);

            read_from_file = (uint8_t*)p_offset;
            write_to_memory = (uint8_t*)p_vaddr;

        
            for(int i = 0; i < p_filesz; i++)
                write_to_memory[i] = read_from_file[i];
                    
        }

        data += 0x20;
        phnum--;
        
    }

    typedef void(*EntryPoint)(char*, char*, char*);
    EntryPoint tmp;
    tmp = entry_point;
    interrupt_disable();
    if(strcmp(argv[0], "elf") || strcmp(argv[0], "elft")) 
    {
        for(int i = 1; i < 5; i++)
            xprintf("\n0x%x", argv[i]);
        
        tmp(argv[1], argv[2], argv[3]);
    }
    else
        tmp(argv[0], argv[1], argv[2]);
    
    free(data);
}

int elfreader(char* filename)
{
    xin_entry* file = fopen(filename, "r");
    elf_load(file);
    return XANIN_OK;
}
