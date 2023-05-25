

#include <lib/libc/stdiox.h>
#include <fs/xin.h>

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

    while(inputg().scan_code != ENTER);


}