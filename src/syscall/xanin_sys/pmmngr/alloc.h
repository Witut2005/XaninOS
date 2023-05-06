
#pragma once

#include <stdint.h>
#include <kernel/pmmngr/alloc.h>

#ifdef __cplusplus
extern "C"{
#endif

void* __sys_calloc(uint32_t size);
void* __sys_realloc(void* ptr, uint32_t size);

static inline void __sys_free(void* ptr)
{
    mmngr_block_free(USER_HEAP, (void*)ptr);
}

static inline void* __sys_malloc(uint32_t size)
{
    return mmngr_block_allocate(USER_HEAP, size);
}

#ifdef __cplusplus
}
#endif