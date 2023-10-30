
#include "./kernel_init.h"

typedef void (*init_section_func_t)(void);

void kernel_execute_init_array_section(const ElfInitArraySectionInfo *const SectionInfo)
{
    uint32_t number_of_entries = SectionInfo->size_of_section / SectionInfo->size_of_section_entry;
    init_section_func_t *functions = (init_section_func_t *)SectionInfo->address;

    for (int i = 0; i < number_of_entries; i++)
        functions[i]();
}

// cant be blank because global dctrs will not be called
void __dso_handle(void)
{
    return;
}

// cant be blank because global dctrs will not be called
void __cxa_atexit(void)
{
    return;
}
