
#include <libc/stdlibx.h>

int memory_test(void)
{
    uint8_t* tmp = (uint8_t*)calloc(SECTOR_SIZE);

    xprintf("memory_test 0x%x\n", tmp);

    while(inputg().scan_code != ENTER);

}