
// http://www.brokenthorn.com/Resources/OSDev17.html

#include <lib/libc/hal.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/devices/com/com.h>
#include <sys/log/syslog.h>
#include <sys/pmmngr/alloc.h>

typedef uint32_t physical_addr;

//! block alignment
#define PMMNGR_BLOCK_ALIGN PMMNGR_BLOCK_SIZE

static bool mmngr_initalized;

uint8_t* mmngr_mmap;        // mmap address
uint32_t mmngr_heap_blocks; // sizeof mmngr available memory space

uint8_t* kernel_heap_base;
uint32_t kernel_heap_offset;
uint32_t kernel_heap_blocks; // 1/3 of allocated memory space belongs to kernel heap

uint8_t* user_heap_base;
uint32_t user_heap_offset;
uint32_t user_heap_blocks; // 2/3 of allocated memory space belongs to user heap

extern "C" {

    static inline void log_invalid_heap(void* ptr)
    {
        char addr_buf[100];
        xsprintf(addr_buf, "0x%x", ptr);
        dbg_warning(DEBUG_LABEL_PMMNGR, strcat(STRCAT_DEST_FIRST, addr_buf, " Invalid free heap. High risk of memory leak"));
    }

    static inline void log_outside_heap(void* ptr)
    {
        char addr_buf[100];
        xsprintf(addr_buf, "0x%x", ptr);
        dbg_warning(DEBUG_LABEL_PMMNGR, strcat(STRCAT_DEST_FIRST, addr_buf, " Memory outside the heap. Are you trying to free stack-allocated memory?"));
    }

    bool mmngr_is_initialized(void)
    {
        return mmngr_initalized;
    }

    static inline uint32_t size_to_blocks_allocated(uint32_t size)
    {
        return (size / PMMNGR_BLOCK_SIZE) + (size % PMMNGR_BLOCK_SIZE == 0 ? 0 : 1);
    }

    uint32_t mmngr_mmap_free_block_find(uint8_t mode, uint32_t blocks)
    {

        if (mode == KERNEL_HEAP)
        {

            for (int i = kernel_heap_offset; i < kernel_heap_offset + kernel_heap_blocks; i++)
            {
                if (mmngr_mmap[i] == MEMORY_UNALLOCATED)
                {

                    if (i + blocks > kernel_heap_offset + kernel_heap_blocks)
                        return UINT32_MAX;

                    bool ok = true;
                    for (int j = i; j < i + blocks; j++)
                    {
                        if (mmngr_mmap[j] != MEMORY_UNALLOCATED)
                            ok = false;
                    }

                    if (ok)
                        return i;
                }
            }

            return UINT32_MAX;
        }

        else // USER HEAP
        {

            for (int i = user_heap_offset; i < user_heap_offset + user_heap_blocks; i++)
            {
                if (mmngr_mmap[i] == MEMORY_UNALLOCATED)
                {
                    if (i + blocks > user_heap_offset + user_heap_blocks)
                        return UINT32_MAX;

                    bool ok = true;
                    for (int j = i; j < i + blocks; j++)
                    {
                        if (mmngr_mmap[j] != MEMORY_UNALLOCATED)
                            ok = false;
                    }

                    if (ok)
                        return i;
                }
            }

            return UINT32_MAX;
        }
    }

    void mmngr_init(uint8_t* map, uint8_t* base, uint32_t blocks)
    {
        mmngr_mmap = map;

        memset(mmngr_mmap, 0, blocks);

        kernel_heap_offset = 0;
        kernel_heap_base = base;
        kernel_heap_blocks = (blocks / 3) + (blocks % 3);

        user_heap_offset = kernel_heap_blocks;
        user_heap_base = &base[user_heap_offset * PMMNGR_BLOCK_SIZE];
        user_heap_blocks = (blocks / 3) * 2;

        mmngr_heap_blocks = blocks;

        for (int i = 0; i < blocks; i++)
            mmngr_mmap[i] = MEMORY_UNALLOCATED;

        mmngr_initalized = true;
        dbg_success(DEBUG_LABEL_PMMNGR, "PMMNGR successully initialized");
    }

    void* mmngr_block_allocate(uint8_t mode, uint32_t size)
    {
        #warning "TODO what happens when size = 0?";
        if (size == 0) size++;

        uint32_t mmap_index = mmngr_mmap_free_block_find(mode, size_to_blocks_allocated(size));

        if (mmap_index == UINT32_MAX) // NO AVAILABLE MEMORY
        {
            dbg_error(DEBUG_LABEL_PMMNGR, "HEAP FULL");
            return (void*)NULL;
        }

        uint32_t blocks_allocated = size_to_blocks_allocated(size);

        for (int i = mmap_index; i < mmap_index + blocks_allocated; i++)
            mmngr_mmap[i] = MEMORY_ALLOCATED;

        mmngr_mmap[mmap_index + blocks_allocated] = MEMORY_ALLOCATED_REGION_END;

        if (mode == KERNEL_HEAP)
            return kernel_heap_base + (mmap_index * PMMNGR_BLOCK_SIZE);

        return user_heap_base + (mmap_index * PMMNGR_BLOCK_SIZE);
    }

    #warning "log invalid heap when kfree on user heap or free on kernel heap";
    void mmngr_block_free(uint8_t mode, void* vptr)
    {
        uint32_t index;
        uint8_t* ptr = (uint8_t*)vptr;

        if (mode == KERNEL_HEAP)
        {
            if (ptr >= user_heap_base && ptr < (user_heap_base + (user_heap_blocks * PMMNGR_BLOCK_SIZE)))
            {
                log_invalid_heap(ptr);
                return;
            }

            else if ((ptr < kernel_heap_base) || (ptr > (kernel_heap_base + (kernel_heap_blocks * PMMNGR_BLOCK_SIZE))))
            {
                log_outside_heap(ptr);
                return;
            }

            index = ((uint32_t)(ptr - kernel_heap_base) / PMMNGR_BLOCK_SIZE);
        }

        else // USER HEAP
        {
            if (ptr >= kernel_heap_base && ptr < (kernel_heap_base + (kernel_heap_blocks * PMMNGR_BLOCK_SIZE)))
            {
                log_invalid_heap(ptr);
                return;
            }

            else if ((ptr < user_heap_base) || (ptr > (user_heap_base + (user_heap_blocks * PMMNGR_BLOCK_SIZE))))
            {
                log_outside_heap(ptr);
                return;
            }

            index = ((uint32_t)(ptr - user_heap_base) / PMMNGR_BLOCK_SIZE);
        }

        if (mmngr_mmap[index] == MEMORY_UNALLOCATED) // already freed
            return;

        for (; mmngr_mmap[index] != MEMORY_ALLOCATED_REGION_END; index++)
            mmngr_mmap[index] = MEMORY_UNALLOCATED;

        mmngr_mmap[index] = MEMORY_UNALLOCATED;
    }

    void* kmalloc(uint32_t size)
    {
        return mmngr_block_allocate(KERNEL_HEAP, size);
    }

    void* kcalloc(uint32_t size)
    {
        if (size == 0) size++;
        uint8_t* tmp = (uint8_t*)mmngr_block_allocate(KERNEL_HEAP, size);
        memset(tmp, 0, size);

        return tmp;
    }

    void kfree(void* ptr)
    {
        mmngr_block_free(KERNEL_HEAP, ptr);
    }

    void* krealloc(void* ptr, uint32_t size)
    {
        if (size == 0) size++;
        EFlags Flags;
        INTERRUPTS_OFF(&Flags);

        interrupt_disable();

        uint8_t* tmp = (uint8_t*)mmngr_block_allocate(KERNEL_HEAP, size);
        memmove(tmp, ptr, size);
        mmngr_block_free(KERNEL_HEAP, ptr);

        INTERRUPTS_ON(&Flags);
        return tmp;
    }

    void* umalloc(uint32_t size)
    {
        return mmngr_block_allocate(USER_HEAP, size);
    }

    void* ucalloc(uint32_t size)
    {
        if (size == 0) size++;
        uint8_t* tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, size);
        memset(tmp, 0, size);

        return tmp;
    }

    void* urealloc(void* ptr, uint32_t size)
    {
        if (size == 0) size++;

        EFlags Flags;
        INTERRUPTS_OFF(&Flags);

        interrupt_disable();

        mmngr_block_free(USER_HEAP, (void*)ptr); // FIRST ALLOCATE THEN FREE (REVERSED ORDER MAKES WEIRD BUGS)
        uint8_t* tmp = (uint8_t*)mmngr_block_allocate(USER_HEAP, size);
        memmove(tmp, (uint8_t*)ptr, size);

        INTERRUPTS_ON(&Flags);
        return tmp;
    }

    void ufree(void* ptr)
    {
        mmngr_block_free(USER_HEAP, ptr);
    }

} //extern "C"

void* operator new(size_t size)
{
    void* p = kcalloc(size);
    return p;
}

void* operator new(size_t size, void* ptr) //placment new
{
    return ptr;
}

void* operator new[](size_t size)
{
    return kcalloc(size);
}

void* operator new[](size_t size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    kfree(ptr);
}

void operator delete(void* ptr, size_t size)
{
    kfree(ptr);
}

void operator delete[](void* ptr)
{
    kfree(ptr);
}

void operator delete[](void* ptr, size_t size)
{
    kfree(ptr);
}