
#pragma once

//http://www.brokenthorn.com/Resources/OSDev17.html


typedef	uint32_t physical_addr;

//! 8 blocks per byte
#define PMMNGR_BLOCKS_PER_BYTE 8
 
//! block size (4k)
#define PMMNGR_BLOCK_SIZE	4096
 
//! block alignment
#define PMMNGR_BLOCK_ALIGN	PMMNGR_BLOCK_SIZE



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