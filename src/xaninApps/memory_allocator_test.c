
#include <libc/stdlibx.h>
#include <libc/alloc.h>

int memory_test(void)
{

    xprintf("base:\n\n");
    xprintf("kernel: 0x%x\n", kernel_heap_base_get());
    xprintf("user: 0x%x\n\n", user_heap_base_get());

    uint8_t* tmp = (uint8_t*)mmngr_block_allocate(KERNEL_HEAP, 1000);
    xprintf("memory_test 0x%x\n", tmp);

    tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, 1000);
    tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, 1000);
    tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, 1000);
    tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, 1000);

    xprintf("memory_test 0x%x\n", tmp);

    while(inputg().scan_code != ENTER);
    return XANIN_OK;

}