

#include <lib/libc/stdiox.h>
#include <fs/xin.h>

#define PMMNGR_MEMORY_BLOCKS 10000

extern uint8_t kernel_mmngr_mmap[PMMNGR_MEMORY_BLOCKS];

int buffers_view(void)
{

    Screen.y = 10;

    float xin_fs_space_used = 0;
    float xin_fs_space_total = (SECTOR_SIZE * XIN_ENTRY_POINTERS_SECTORS);

    for(uint8_t* sector = (uint8_t*)XIN_ENTRY_POINTERS; sector < (uint8_t*)(XIN_ENTRY_POINTERS + (SECTOR_SIZE * XIN_ENTRY_POINTERS_SECTORS)); sector++)
    {
        if(*sector != XIN_UNALLOCATED)
            xin_fs_space_used++;
    }

    xprintf("used: %d\n", (uint32_t)xin_fs_space_used);
    xprintf("total: %d\n", (uint32_t)xin_fs_space_total);
    
    xprintf("XinFs space used:  [");

    for(int i = 0; i < 50; i++)
    {
        if((i * 2) < (xin_fs_space_used / xin_fs_space_total) * 100)
            xprintf("%z-", OUTPUT_COLOR_SET(black, red));

        else
            xprintf("%z-", OUTPUT_COLOR_SET(black, green));

    }

    xprintf("]");

    xprintf("\n\n\n");

    float heap_space_used = 0;
    float heap_space_total = PMMNGR_MEMORY_BLOCKS;

    for(uint8_t* block = kernel_mmngr_mmap; (uint32_t)block < (uint32_t)&kernel_mmngr_mmap[PMMNGR_MEMORY_BLOCKS]; block++)
    {
        if(*block != MEMORY_UNALLOCATED)
            heap_space_used++;
    }

    xprintf("used: %d\n", (uint32_t)heap_space_used);
    xprintf("total: %d\n", (uint32_t)heap_space_total);
    
    xprintf("heap space used:  [");

    for(int i = 0; i < 50; i++)
    {
        if((i * 2) < (heap_space_used / heap_space_total) * 100)
            xprintf("%z-", OUTPUT_COLOR_SET(black, red));

        else
            xprintf("%z-", OUTPUT_COLOR_SET(black, green));

    }

    xprintf("]");





    while(inputg().scan_code != ENTER);


}