
//http://www.brokenthorn.com/Resources/OSDev17.html

#include <stdint.h>
#include <stddef.h>
#include <libc/memory.h>
#include <libc/alloc.h>

typedef	uint32_t physical_addr;

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8
 
//! block size (4k)
#define PMMNGR_BLOCK_SIZE	4096
 
//! block alignment
#define PMMNGR_BLOCK_ALIGN	PMMNGR_BLOCK_SIZE
//! size of physical memory
uint32_t	_mmngr_memory_size=0;
//! number of blocks currently in use
uint32_t	_mmngr_used_blocks=0;
//! maximum number of available memory blocks
uint32_t	_mmngr_max_blocks=0;
//! memory map bit array. Each bit represents a memory block
uint32_t*	_mmngr_memory_map= 0;

size_t	pmmngr_get_memory_size () 
{
	return _mmngr_memory_size;
}

uint32_t pmmngr_get_free_block_count () 
{
	return _mmngr_max_blocks - _mmngr_used_blocks;
}

uint32_t pmmngr_get_block_count () 
{
	return _mmngr_max_blocks;
}

void mmap_set (int bit) 
{ 
    _mmngr_memory_map[bit / 32] |= (1 << (bit % 32));
}

void mmap_unset (int bit) 
{ 
    _mmngr_memory_map[bit / 32] &= ~ (1 << (bit % 32));
}

bool mmap_test (int bit) 
{
    return _mmngr_memory_map[bit / 32] &  (1 << (bit % 32));
}


int mmap_first_free () 
{ 
	//! find the first free bit
	  for (uint32_t i = 0x10; i< pmmngr_get_block_count() / 32; i++)
        if (_mmngr_memory_map[i] != 0xffffffff)
			      for (int j=0; j<32; j++) 
            {		//! test each bit in the dword
				        int bit = 1 << j;
				    if (! (_mmngr_memory_map[i] & bit) )
					  return i*4*8+j;
			      }
 
	return -1;
}

void pmmngr_init (size_t memSize, physical_addr bitmap) 
{ 
    _mmngr_memory_size	=	memSize;
    _mmngr_memory_map	=	(uint32_t*) bitmap;
    _mmngr_max_blocks	=	(pmmngr_get_memory_size()*1024) / PMMNGR_BLOCK_SIZE;
    _mmngr_used_blocks	=	pmmngr_get_block_count();

    //! By default, all of memory is in use
    memset ((uint8_t*)_mmngr_memory_map, 0xf, pmmngr_get_block_count() / PMMNGR_BLOCKS_PER_BYTE );
}


void pmmngr_init_region (physical_addr base, size_t size) 
{
 
    int align = base / PMMNGR_BLOCK_SIZE;
    int blocks = size / PMMNGR_BLOCK_SIZE;
 
    for (; blocks>0; blocks--) 
    {
		    mmap_unset (align++);
		    _mmngr_used_blocks--;
	  }
 
	  mmap_set (0);	//first block is always set. This insures allocs cant be 0
}


void pmmngr_deinit_region (physical_addr base, size_t size) 
{

    int align = base / PMMNGR_BLOCK_SIZE;
    int blocks = size / PMMNGR_BLOCK_SIZE;

    for (; blocks>0; blocks--) 
    {
        mmap_set (align++);
        _mmngr_used_blocks++;
    }
}

void* pmmngr_alloc_block () {
 
    if (pmmngr_get_free_block_count() <= 0)
        return 0;	//out of memory

    int frame = mmap_first_free ();

    if (frame == -1)
        return 0;	//out of memory

    mmap_set (frame);

    physical_addr addr = frame * PMMNGR_BLOCK_SIZE;
    _mmngr_used_blocks++;

    return (void*)addr;
}

void pmmngr_free_block (void* p) 
{
    physical_addr addr = (physical_addr)p;
	  int frame = addr / PMMNGR_BLOCK_SIZE;
 
	  mmap_unset (frame);
 
	  _mmngr_used_blocks--;
}


////////////////////////////////////////


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

uint32_t mmngr_mmap_free_block_find(uint8_t mode)
{

    
    if(mode == KERNEL_HEAP)
    {

        for(int i = kernel_heap_offset; i < kernel_heap_offset + kernel_heap_blocks; i++)
        {
            if(mmngr_mmap[i] == MEMORY_UNALLOCATED) 
                return i;
        }

        return UINT32_MAX;

    }

    else //USER HEAP
    {

        for(int i = user_heap_offset; i < user_heap_offset + user_heap_blocks; i++)
        {
            if(mmngr_mmap[i] == MEMORY_UNALLOCATED) 
                return i;
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

    uint32_t mmap_index = mmngr_mmap_free_block_find(mode);

    if(mmap_index == UINT32_MAX) // NO AVAILABLE MEMORY
    {
        xprintf("EERRORORORORO");
        return (void*)NULL;
    }

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