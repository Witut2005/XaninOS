
//http://www.brokenthorn.com/Resources/OSDev17.html

#include <stdint.h>
#include <stddef.h>
#include <libc/memory.h>
#include <kernel/pmmngr/alloc.h>

typedef	uint32_t physical_addr;

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8
 
//! block size (4k)
#define PMMNGR_BLOCK_SIZE	4096
 
//! block alignment
#define PMMNGR_BLOCK_ALIGN	PMMNGR_BLOCK_SIZE


uint8_t* mmngr_mmap;            //mmap address
uint32_t mmngr_heap_blocks;       //sizeof mmngr available memory space

uint8_t* kernel_heap_base;
uint32_t kernel_heap_offset;    
uint32_t kernel_heap_blocks;      // 1/3 of allocated memory space belongs to kernel heap

uint8_t* user_heap_base;
uint32_t user_heap_offset;      
uint32_t user_heap_blocks;      // 2/3 of allocated memory space belongs to user heap

static inline uint32_t size_to_blocks_allocated(uint32_t size)
{
    return (size / PMMNGR_BLOCK_SIZE) + (size % PMMNGR_BLOCK_SIZE == 0 ? 0 : 1); 
}

uint32_t mmngr_mmap_free_block_find(uint8_t mode, uint32_t blocks)
{

    
    if(mode == KERNEL_HEAP)
    {

        for(int i = kernel_heap_offset; i < kernel_heap_offset + kernel_heap_blocks; i++)
        {
            if(mmngr_mmap[i] == MEMORY_UNALLOCATED) 
            {

                if(i + blocks > kernel_heap_offset + kernel_heap_blocks)
                    return UINT32_MAX;

                bool ok = true;
                for(int j = i; j < i + blocks; j++)
                {
                    if(mmngr_mmap[j] != MEMORY_UNALLOCATED)
                        ok = false;
                }
                
                if(ok)
                    return i;
            }
        }

        return UINT32_MAX;

    }

    else //USER HEAP
    {

        for(int i = user_heap_offset; i < user_heap_offset + user_heap_blocks; i++)
        {
            if(mmngr_mmap[i] == MEMORY_UNALLOCATED) 
            {

                if(i + blocks > user_heap_offset + user_heap_blocks)
                    return UINT32_MAX;

                bool ok = true;
                for(int j = i; j < i + blocks; j++)
                {
                    if(mmngr_mmap[j] != MEMORY_UNALLOCATED)
                        ok = false;
                }
                
                if(ok)
                    return i;
            }
        }

        return UINT32_MAX;

    }

}

void mmngr_init(uint8_t* map, uint8_t* base, uint32_t blocks)
{
    mmngr_mmap = map;

    kernel_heap_offset = 0;
    kernel_heap_base = base;
    kernel_heap_blocks = (blocks / 3) + (blocks % 3);

    user_heap_offset = kernel_heap_blocks;
    user_heap_base = &base[user_heap_offset * PMMNGR_BLOCK_SIZE];
    user_heap_blocks = (blocks / 3) * 2;

    mmngr_heap_blocks = blocks;

    for(int i = 0; i < blocks; i++)
        mmngr_mmap[i] = MEMORY_UNALLOCATED;

    for(int i = 0; i < blocks * PMMNGR_BLOCK_SIZE; i++)
        base[i] = 0;

}

void* mmngr_block_allocate(uint8_t mode, uint32_t size)
{

    uint32_t mmap_index = mmngr_mmap_free_block_find(mode, size_to_blocks_allocated(size));

    if(mmap_index == UINT32_MAX) // NO AVAILABLE MEMORY
        return (void*)NULL;

    uint32_t blocks_allocated = size_to_blocks_allocated(size);

    // xprintf("index: %d allocated: %d\n", mmap_index, blocks_allocated);

    for(int i = mmap_index; i < mmap_index + blocks_allocated - 1; i++)
        mmngr_mmap[i] = MEMORY_ALLOCATED;

    mmngr_mmap[mmap_index + blocks_allocated - 1] = MEMORY_ALLOCATED_REGION_END;
        
    if(mode == KERNEL_HEAP)
        return kernel_heap_base + (mmap_index * PMMNGR_BLOCK_SIZE);

    return user_heap_base + (mmap_index * PMMNGR_BLOCK_SIZE);

}


void mmngr_block_free(uint8_t mode, void* ptr)
{
    uint32_t index; 

    if(mode == KERNEL_HEAP)
        index = ((uint32_t)((uint32_t)ptr - (uint32_t)kernel_heap_base) / PMMNGR_BLOCK_SIZE);

    else //USER HEAP
        index = ((uint32_t)((uint32_t)ptr - (uint32_t)user_heap_base) / PMMNGR_BLOCK_SIZE);

    // xprintf("free: %d\n", index);

    for(; mmngr_mmap[index] != MEMORY_ALLOCATED_REGION_END; index++)
        mmngr_mmap[index] = MEMORY_UNALLOCATED;
    
    mmngr_mmap[index] = MEMORY_UNALLOCATED;

}

void* kmalloc(uint32_t size)
{
    return mmngr_block_allocate(KERNEL_HEAP, size);
}

void* kcalloc(uint32_t size)
{
    uint8_t* tmp = mmngr_block_allocate(KERNEL_HEAP, size);

    for(int i = 0; i < size; i++)
        tmp[i] = 0;

    return tmp;
}

void kfree(void * ptr)
{
    mmngr_block_free(KERNEL_HEAP, ptr);
}

void* krealloc(void* ptr, uint32_t size)
{
    // interrupt_disable();

    uint8_t* tmp = mmngr_block_allocate(KERNEL_HEAP, size);
    memcpy(tmp, ptr, size);
    mmngr_block_free(KERNEL_HEAP, ptr);

    // interrupt_enable();

    return tmp;
}