
#include "./elf.h"
#include "./bootio.h"
#include "./disk.h"
#include "./string.h"

char* elf_section_header_string_table_address_get(ElfAutoHeader* Header)
{
    ElfSectionHeaderAuto* SectionHeaders = (ElfSectionHeaderAuto*)(Header->e_shoff + (uint32_t)Header);
    return (char*)(SectionHeaders[Header->e_shstrndx].sh_offset + (uint32_t)Header);
}

bool elf_load_given_section(ElfAutoHeader* Header, const char* section_name)
{
    char* elf_section_names = elf_section_header_string_table_address_get(Header);
    ElfSectionHeaderAuto* Sections = (ElfSectionHeaderAuto*)(Header->e_shoff + (uint32_t)Header);

    vga_screen_buffer_clear();

    for(int i = 0; i < Header->e_shnum; i++)
    {
        if(bstrcmp(section_name, &elf_section_names[Sections[i].sh_name]))
        {
            for(int j = 0; j < Sections[i].sh_size; j++)
                ((uint8_t*)(Sections[i].sh_addr))[j] = ((uint8_t*)(Sections[i].sh_offset + (uint32_t)(Header)))[j];

            return true;
        }

    }
    return false;
}

void elf_load(void)
{
    uint8_t* data = (uint8_t*)(0x20200 + (15 * SECTOR_SIZE));
    
    uint8_t* write_to_memory;
    uint8_t* read_from_file;
    uint32_t begin_of_code = (uint32_t)data;

    uint16_t phnum = *(uint16_t*)((uint8_t*)data + 0x2C);

    uint32_t p_offset;      //offset in file image
    uint32_t p_vaddr;       //virtual address of the segment in memory
    uint32_t p_filesz;      //size in bytes of segment in file image
    uint32_t entry_point = *(uint32_t*)((uint8_t*)data + 0x18);

    data += 0x34;

    print_decimal(phnum, OUTPUT_COLOR_SET(black, white));

    while(phnum)
    {

        if(*(uint32_t*)data == PT_LOAD)
        {
            print("PT LOAD FINDED", OUTPUT_COLOR_SET(black, white));
            putchar('\n', BOOTIO_NO_COLOR);

            p_offset = *(uint32_t*)(data + 0x4);
            p_offset = p_offset + begin_of_code;
            p_vaddr  = *(uint32_t*)(data + 0x8);
            p_filesz = *(uint32_t*)(data + 0x10);

            print_hex(p_offset, OUTPUT_COLOR_SET(black, white));
            putchar('\n', BOOTIO_NO_COLOR);
            print_hex(p_vaddr, OUTPUT_COLOR_SET(black, white));
            putchar('\n', BOOTIO_NO_COLOR);

            read_from_file = (uint8_t*)p_offset;
            write_to_memory = (uint8_t*)p_vaddr;

            for(int i = 0; i < p_filesz; i++, write_to_memory++, read_from_file++)
                *write_to_memory = *read_from_file;
            
        }

        data += 0x20;
        phnum--;
    }
    
    print("kernel loaded", OUTPUT_COLOR_SET(black, white));
    putchar('\n', BOOTIO_NO_COLOR);

    elf_load_given_section((ElfAutoHeader*)(0x20200 + (15 * SECTOR_SIZE)), ".init_array");

    void(*kernel)(void) = (void(*)(void))entry_point;
    kernel();
}
