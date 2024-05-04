
#include <stdint.h>
#include <lib/libc/stdiox.h>

extern uint32_t page_directory[1024];

__STATUS paging_test(void)
{
    for (int i = 0; i < 32; i++) {
        xprintf("%d. 0x%b\n", i, page_directory[i] >> 22);
    }
    return XANIN_OK;
}