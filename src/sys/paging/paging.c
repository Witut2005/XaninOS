
#include "./paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

void page_directory_entry_set(uint32_t index, PageDirectoryEntry4MB* data)
{
    page_directory[index] = data->fields;
}
