
#include "./paging.h"

uint32_t page_directory[XANIN_NUMBER_OF_PAGE_DIRECTORIES] __attribute__((aligned(4096)));

#warning "TO DO PAGING"

void page_directory_entry_set(uint32_t index, uint32_t page_base_address)
{
    PageDirectoryEntry4MB page_dir_entry = { {PAGE_DIRECTORY4MB_CREATE(page_base_address)} }; // kernel page
    page_directory[index] = page_dir_entry.fields;
}
