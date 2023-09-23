
#pragma once

#include <stdint.h>

struct ElfInitArraySectionInfo
{
    uint32_t address;
    uint32_t size_of_section; 
    uint32_t size_of_section_entry;
};
typedef struct ElfInitArraySectionInfo ElfInitArraySectionInfo;

#ifdef __cplusplus
extern "C" {
#endif

void kernel_execute_init_array_section(const ElfInitArraySectionInfo* const SectionInfo);


#ifdef __cplusplus
}
#endif