
#include <sys/call/xanin_sys/pmmngr/alloc.h>
#include <lib/libc/memory.h>
#include <lib/libc/hal.h>

void* __sys_calloc(uint32_t size)
{
    uint8_t* tmp = mmngr_block_allocate(USER_HEAP, size);
    memset(tmp, 0, size);

    return tmp;

}


void* __sys_realloc(void* ptr, uint32_t size)
{
    mmngr_block_free(USER_HEAP, (void*)ptr);
    uint8_t* tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, size);

    if((uint32_t)tmp != (uint32_t)ptr)
        memmove(tmp, (uint8_t*)ptr, size);

    return tmp;
}