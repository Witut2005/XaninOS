
#include "./paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

#warning "TO DO PAGING"

void page_directory_entry_set(uint32_t index, uint32_t address)
{
    PageDirectoryEntry4MB page_dir_entry = { {PAGE_DIRECTORY4MB_CREATE(address)} }; // kernel page
    page_directory[index] = page_dir_entry.fields;
}
