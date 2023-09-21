
#include "./elf.h"
#include "./bootio.h"
#include "./disk.h"
#include "./string.h"
#include "./memory.h"

char* elf_section_header_string_table_address_get(ElfHeaderAuto* Header)
{
    ElfSectionHeaderAuto* KernelSectionHeaders = (ElfSectionHeaderAuto*)(Header->e_shoff + XANIN_OS_KERNEL_MEMORY_LOCATION);
    return (char*)(KernelSectionHeaders[Header->e_shstrndx].sh_offset + XANIN_OS_KERNEL_MEMORY_LOCATION);
}

bool elf_load_given_section(ElfHeaderAuto* KernelHeader, const char* section_name)
{
    char* elf_section_names = elf_section_header_string_table_address_get(KernelHeader);
    ElfSectionHeaderAuto* KernelSectionsHeaders = (ElfSectionHeaderAuto*)(KernelHeader->e_shoff + XANIN_OS_KERNEL_MEMORY_LOCATION);

    for(int i = 0; i < KernelHeader->e_shnum; i++)
    {
    if(bstrcmp(section_name, &elf_section_names[KernelSectionsHeaders[i].sh_name]))
        {
            memcpy((uint8_t*)KernelSectionsHeaders[i].sh_addr, 
                (uint8_t*)(KernelSectionsHeaders[i].sh_offset + XANIN_OS_KERNEL_MEMORY_LOCATION), KernelSectionsHeaders[i].sh_size);
            return true;
        }

    }
    return false;
}

void elf_load(void)
{
    ElfHeaderAuto* KernelHeader = (ElfHeaderAuto*)XANIN_OS_KERNEL_MEMORY_LOCATION;
    ElfProgramHeaderAuto* KernelProgramHeaders = (ElfProgramHeaderAuto*)(XANIN_OS_KERNEL_MEMORY_LOCATION + sizeof(ElfHeaderAuto));

    print("elf magic: %s\n", KernelHeader->ei_mag);
    print("phnum: %d\n", KernelHeader->e_phnum);

    for(int i = 0; i < KernelHeader->e_phnum; i++)
    {
        if(KernelProgramHeaders[i].p_type == PT_LOAD)
        {
            memcpy((uint8_t*)KernelProgramHeaders[i].p_vaddr, 
                (uint8_t*)(KernelProgramHeaders[i].p_offset + XANIN_OS_KERNEL_MEMORY_LOCATION), KernelProgramHeaders[i].p_filesz);
        }
    }
    
    print("%hkernel loaded ^^\n", OUTPUT_COLOR_SET(black, green));

    elf_load_given_section((ElfHeaderAuto*)(XANIN_OS_KERNEL_MEMORY_LOCATION),".init_array");

    void(*kernel)(void) = (void(*)(void))KernelHeader->e_entry;
    kernel();
}
