
#include <lib/libc/stdlibx.h>
#include <sys/pmmngr/alloc.h>

int memory_test(void)
{

    xprintf("mmngr map kernel space: 0x%x\n", mmngr_mmap);
    xprintf("mmngr map user space: 0x%x\n", &mmngr_mmap[user_heap_offset]);

    xprintf("base\n");
    xprintf("kernel: 0x%x\n", kernel_heap_base_get());
    xprintf("user: 0x%x\n\n", user_heap_base_get());

    uint8_t* tmp = (uint8_t*)malloc(100);
    xprintf("0x%x\n", tmp);

    tmp = (uint8_t*)malloc(0xFFFFFFFF);
    xprintf("0x%x\n", tmp);


    while(inputg().scan_code != ENTER);
    return XANIN_OK;

}
