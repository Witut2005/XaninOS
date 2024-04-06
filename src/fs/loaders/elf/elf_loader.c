
#include <fs/loaders/elf/elf_loader.h>
#include <lib/elf/elf.h>
#include <lib/libc/file.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <stddef.h>
#include <sys/devices/com/com.h>
#include <sys/input/input.h>

extern char *argv[5]; // USE HERE SYSCALL

bool elf_check_magic(uint8_t *data)
{
    return bstrncmp((char *)data + 1, "ELF", 3); // && data[0] == 0x7F;
}

bool elf_check_arch(uint8_t *file)
{
    uint16_t tmp;

    tmp = file[0];
    tmp += file[1] << 8;

    return tmp == X86_E_MACHINE;
}

void elf_load(XinEntry *file)
{
    uint8_t *data = (uint8_t *)calloc(file->size);

    if (data == NULL)
    {
        xprintf("MEMORY OUT");
        while (getxchar().scan_code != ENTER)
            ;
        return;
    }

    if (data == NULL)
        dbg_error(DEBUG_LABEL_PMMNGR, "MEMORY OUT");

    dbg_info(DEBUG_LABEL_PROCESS, "ELF data reading");
    fread(file, data, file->size);
    dbg_info(DEBUG_LABEL_PROCESS, "ELF data readed");

    // xprintf("file size: %d\n", file->size);
    // xprintf("press ENTER to start: ");
    // while(getxchar().scan_code != ENTER);

    uint8_t *write_to_memory;
    uint8_t *read_from_file;
    uint32_t file_base = (uint32_t)data;

    uint16_t phnum = *(uint16_t *)((uint8_t *)data + 0x2C);

    uint32_t p_offset; // offset in file image
    uint32_t p_vaddr;  // virtual address of the segment in memory
    uint32_t p_filesz; // size in bytes of segment in file image
    // uint32_t p_memsz;       //size in bytes of segment in memory
    uint32_t entry_point = *(uint32_t *)((uint8_t *)data + 0x18);

    if (!elf_check_magic(data))
    {
        xprintf("%zBAD MAGIC\n", OUTPUT_COLOR_ERROR_SET);
        while (getxchar().scan_code != ENTER)
            ;
        return;
    }

    if (!elf_check_arch(data + 0x12))
    {
        xprintf("%zBAD ARCH\n", OUTPUT_COLOR_ERROR_SET);
        while (getxchar().scan_code != ENTER)
            ;
        return;
    }

    dbg_success(DEBUG_LABEL_PROCESS, "Valid ELF file");

    data += ELF_HEADER_SIZE;
    uint32_t load_sum = 0;

    while (phnum)
    {
        if (*(uint32_t *)data == PT_LOAD)
        {
            load_sum++;

            p_offset = *(uint32_t *)((uint8_t *)data + 0x4) + file_base;
            p_vaddr = *(uint32_t *)((uint8_t *)data + 0x8);
            p_filesz = *(uint32_t *)((uint8_t *)data + 0x10);
            // p_memsz  = *(uint32_t*)((uint8_t*)data + 0x14);

            read_from_file = (uint8_t *)p_offset;
            write_to_memory = (uint8_t *)p_vaddr;

            for (int i = 0; i < p_filesz; i++)
                write_to_memory[i] = read_from_file[i];
        }

        data += 0x20;
        phnum--;
    }

    dbg_success(DEBUG_LABEL_PROCESS, "ELF file loaded");

    if (bstrcmp(argv[0], "elf") || bstrcmp(argv[0], "elft"))
        ((void (*)(void))entry_point)();
    else
        ((void (*)(void))entry_point)();

    free(data);
}

int elfreader(char *filename)
{
    XinEntry *file = fopen(filename, "r");

    if (file == NULL)
    {
        xprintf("%zNO SUCH FILE\n", OUTPUT_COLOR_ERROR_SET);
        while (getxchar().scan_code != ENTER)
            ;
        return XANIN_ERROR;
    }

    elf_load(file);
    return XANIN_OK;
}
