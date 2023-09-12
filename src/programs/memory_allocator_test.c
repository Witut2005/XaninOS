
#include <lib/libc/stdlibx.h>
#include <sys/pmmngr/alloc.h>

//TERMINAL_APP

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


    return XANIN_OK;
}
