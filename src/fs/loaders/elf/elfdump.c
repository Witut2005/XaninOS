
#include <stddef.h>
#include <fs/xin.h>
#include <lib/elf/elf.h>
#include <sys/input/input.h>
#include <lib/libc/stdiox.h>

void elf_data_load(XinEntry* file)
{
    uint8_t* data = (uint8_t*)calloc(file->size);
    fread(file, data, file->size * SECTOR_SIZE);
    char* magic = data;
    
    uint32_t file_base = (uint32_t)data;

    uint16_t phnum = *(uint16_t*)((uint8_t*)data + 0x2C);

    uint32_t p_offset;      //offset in file image
    uint32_t p_vaddr;       //virtual address of the segment in memory
    uint32_t p_filesz;      //size in bytes of segment in file image
    uint32_t p_memsz;       //size in bytes of segment in memory
    uint32_t entry_point = *(uint32_t*)((uint8_t*)data + 0x18);

    data += ELF_HEADER_SIZE;
    uint32_t load_sum = 0;

    xprintf("%zExecutable and Linkage File Format Info:\n", OUTPUT_COLOR_SET(green, white));
    xprintf("--------------------------\n");
    xprintf("Magic: %s\n", magic);
    xprintf("--------------------------\n");
    xprintf("Entry point: 0x%x\n", entry_point);
    xprintf("--------------------------\n");

    while(phnum)
    {
        
        if(*(uint32_t*)data == PT_LOAD)
        {

            load_sum++;

            p_offset = *(uint32_t*)((uint8_t*)data + 0x4) + file_base;
            p_vaddr  = *(uint32_t*)((uint8_t*)data + 0x8);
            p_filesz = *(uint32_t*)((uint8_t*)data + 0x10);
            p_memsz  = *(uint32_t*)((uint8_t*)data + 0x14);

            xprintf("p_offset: 0x%x\n", p_offset);
            xprintf("--------------------------\n");
            xprintf("p_vaddr: 0x%x\n", p_vaddr);
            xprintf("--------------------------\n");
            xprintf("p_filesz: 0x%x\n", p_filesz);
            xprintf("--------------------------\n");
            xprintf("p_memsz: 0x%x\n", p_memsz);           
            xprintf("--------------------------\n");
        }

        data += 0x20;
        phnum--;
        
    }



    
    while(getxchar().scan_code != ENTER);
    free(data);
}

int elfdump(char* filename)
{
    XinEntry* file = fopen(filename, "r");

    if(file == NULL)
    {
        xprintf("%zError: Couldn't open file '%s'\n", OUTPUT_COLOR_SET(red, white), filename);
        return XANIN_ERROR;
    }

    elf_data_load(file);
    return XANIN_OK;
}
