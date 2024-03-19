
#pragma once

//CR0.PG = 1
//CR4.PAE = 0
//CR4.PSE

#include <stdint.h>

enum PAGE_DIRECTORY_ENTRY_4MB
{
    PAGE_DIRECTORY_PRESENT = 1,
    PAGE_DIRECTORY_RW = 1 << 1,
    PAGE_DIRECTORY_US = 1 << 2,
    PAGE_DIRECTORY_PWT = 1 << 3,
    PAGE_DIRECTORY_PCD = 1 << 4,
    PAGE_DIRECTORY_A = 1 << 5,
    PAGE_DIRECTORY_D = 1 << 6,
    PAGE_DIRECTORY_PS = 1 << 7,
    PAGE_DIRECTORY_G = 1 << 8
};

uint32_t cpu_paging_related_info_get(void);