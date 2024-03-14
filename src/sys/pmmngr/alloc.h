
#pragma once

//http://www.brokenthorn.com/Resources/OSDev17.html

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef	uint32_t physical_addr;

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8
 
//! block size (4k)
#define PMMNGR_BLOCK_SIZE 512
 
//! block alignment
#define PMMNGR_BLOCK_ALIGN	PMMNGR_BLOCK_SIZE

////////////////////////////////////////////////////////////////

extern uint8_t* mmngr_mmap;            //mmap address
extern uint32_t mmngr_mmap_blocks;       //SIZE_OF mmngr available memory space

extern uint8_t* kernel_heap_base;
extern uint32_t kernel_heap_offset;    
extern uint32_t kernel_heap_blocks;      // 1/3 of allocated memory space belongs to kernel heap

extern uint8_t* user_heap_base;
extern uint32_t user_heap_offset;      
extern uint32_t user_heap_blocks;      // 2/3 of allocated memory space belongs to user heap

enum ALLOCATION_MODES
{
    KERNEL_HEAP = 0,
    USER_HEAP = 1
};

enum MEMORY_ALLOCATOR_BLOCKS_STATES
{
    MEMORY_UNALLOCATED = 0,
    MEMORY_ALLOCATED = 1,
    MEMORY_ALLOCATED_REGION_END = 0xFF
};

static inline uint8_t* user_heap_base_get(void)
{
    return user_heap_base;
}

static inline uint8_t* kernel_heap_base_get(void)
{
    return kernel_heap_base;
}


#ifdef __cplusplus
extern "C" {
#endif

bool mmngr_is_initialized(void);
uint32_t mmngr_mmap_free_block_find(uint8_t mode, uint32_t blocks);
void mmngr_init(uint8_t* map, uint8_t* base, uint32_t size);
void* mmngr_block_allocate(uint8_t mode, uint32_t size);
void mmngr_block_free(uint8_t mode, void* ptr);
void* kmalloc(uint32_t size);
void* kcalloc(uint32_t size);
void kfree(void* ptr);
void* krealloc(void* ptr, uint32_t size);

#ifdef __cplusplus
}
#endif