
#pragma once

// http://www.brokenthorn.com/Resources/OSDev17.html

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PMMNGR_MEMORY_BLOCKS 15000

typedef uint32_t physical_addr;

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8

//! block size (4k)
#define PMMNGR_BLOCK_SIZE 512

//! block alignment
#define PMMNGR_BLOCK_ALIGN PMMNGR_BLOCK_SIZE

////////////////////////////////////////////////////////////////

enum ALLOCATION_MODES {
    KERNEL_HEAP = 0,
    USER_HEAP = 1
};

enum MEMORY_ALLOCATOR_BLOCKS_STATES {
    MEMORY_UNALLOCATED = 0,
    MEMORY_ALLOCATED = 1,
    MEMORY_ALLOCATED_REGION_END = 0xFF
};



#ifdef __cplusplus
extern "C" {
#endif

    uint8_t* user_heap_base_get(void);
    uint8_t* kernel_heap_base_get(void);
    bool mmngr_is_initialized(void);
    uint32_t mmngr_mmap_free_block_find(uint8_t mode, uint32_t blocks);
    void mmngr_init(uint8_t* map, uint8_t* base, uint32_t size);
    void* mmngr_block_allocate(uint8_t mode, uint32_t size);
    void mmngr_block_free(uint8_t mode, void* ptr);

    void* kmalloc(uint32_t size);
    void* kcalloc(uint32_t size);
    void kfree(void* ptr);
    void* krealloc(void* ptr, uint32_t size);

    void* umalloc(uint32_t size);
    void* ucalloc(uint32_t size);
    void ufree(void* ptr);
    void* urealloc(void* ptr, uint32_t size);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

void* operator new(size_t size);
void* operator new(size_t size, void* ptr); // placment new

void* operator new[](size_t size);
void* operator new[](size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete(void* ptr, size_t size);

void operator delete[](void* ptr);
void operator delete[](void* ptr, size_t size);

#endif
