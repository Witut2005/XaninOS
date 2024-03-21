
#include "./paging.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));

void page_directory_entry_set(uint32_t index, PageDirectoryEntry4MB* data)
{
    page_directory[index] = data->fields;
}

void page_directory_init(uint32_t* pages, uint32_t size)
{
    // for (int i = 0; i < size; i++)
    //     page_directory[i] = pages
}

