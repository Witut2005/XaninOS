
#pragma once

//http://www.brokenthorn.com/Resources/OSDev17.html


typedef	uint32_t physical_addr;

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8
 
//! block size (4k)
#define PMMNGR_BLOCK_SIZE	4096
 
//! block alignment
#define PMMNGR_BLOCK_ALIGN	PMMNGR_BLOCK_SIZE

extern uint32_t	_mmngr_memory_size;
//! number of blocks currently in use
extern uint32_t	_mmngr_used_blocks;
//! maximum number of available memory blocks
extern uint32_t	_mmngr_max_blocks;
//! memory map bit array. Each bit represents a memory block
extern uint32_t*	_mmngr_memory_map;


size_t	pmmngr_get_memory_size ();
uint32_t pmmngr_get_free_block_count ();
uint32_t pmmngr_get_block_count ();
void mmap_set (int bit);
void mmap_unset (int bit); 
bool mmap_test (int bit);
int mmap_first_free ();
void pmmngr_init (size_t memSize, physical_addr bitmap); 
void pmmngr_init_region (physical_addr base, size_t size);
void pmmngr_deinit_region (physical_addr base, size_t size); 
void* pmmngr_alloc_block ();
void pmmngr_free_block (void* p); 



////////////////////////////////////////////////////////////////

extern uint8_t* mmngr_mmap;            //mmap address
extern uint32_t mmngr_mmap_blocks;       //sizeof mmngr available memory space

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

static inline int8_t* user_heap_base_get(void)
{
    return user_heap_base;
}

static inline uint8_t* kernel_heap_base_get(void)
{
    return kernel_heap_base;
}



uint32_t mmngr_mmap_free_block_find(uint8_t mode, uint32_t blocks);
void mmngr_init(uint8_t* map, uint8_t* base, uint32_t size);
// void mmngr_init_region(uint32_t offset, uint32_t size);
// void mmngr_deinit_region(uint32_t offset, uint32_t size);
void* mmngr_block_allocate(uint8_t mode, uint32_t size);
void mmngr_block_free(uint8_t mode, void* ptr);