
#include "./paging.h"
#include <cpuid.h>

// static uint32_t page_directory[1024] __attribute__((aligned(4096)));

uint32_t cpu_paging_related_info_get(void)
{
    unsigned int unused;
    unsigned int result;
    __get_cpuid(1, &unused, &unused, &unused, &result);
    return result;
}

void page_directory_init(uint32_t* pages, uint32_t size)
{
    // for (int i = 0; i < size; i++)
    //     page_directory[i] = pages
}