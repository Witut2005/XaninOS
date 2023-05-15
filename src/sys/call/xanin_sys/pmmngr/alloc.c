
#include <sys/call/xanin_sys/pmmngr/alloc.h>
#include <lib/libc/memory.h>

void* __sys_calloc(uint32_t size)
{
    uint8_t* tmp = mmngr_block_allocate(USER_HEAP, size);
    memset(tmp, 0, size);

    return tmp;

}


void* __sys_realloc(void* ptr, uint32_t size)
{
    
    uint8_t* tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, size);
    mmngr_block_free(USER_HEAP, (void*)ptr);

    // if(tmp != (uint8_t*)ptr)
        memcpy(tmp, (uint8_t*)ptr, size);

    return tmp;

}