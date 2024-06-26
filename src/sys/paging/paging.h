
#pragma once


#include <stdint.h>
#include <stdbool.h>
#include <lib/libc/hal.h>
#include <lib/libc/memory.h>

#define CPUID_PAE (1 << 6)
#define CPUID_PSE36 (1 << 17)
#define PAGE_DIRECTORY4MB_CREATE(addr)(0x001E7 | (addr & 0xFFC00000))
#define PAGE_FRAME_UPPER_BITS_OFFSET 22

#define XANIN_NUMBER_OF_PAGE_DIRECTORIES 1024 
#define XANIN_KERNEL_PAGE_DIRECTORIES_INDEX 0x3
#define XANIN_NUMBER_OF_KERNEL_PAGE_DIRECTORIES 32

#if XANIN_NUMBER_OF_PAGE_DIRECTORIES != 1024 
#error  "Number of pages must be equal to 4096"
#endif

#if XANIN_NUMBER_OF_KERNEL_PAGE_DIRECTORIES < 32
#error "Number of XaninPageDirectories must be greater of equal to 32"
#endif

#if XANIN_KERNEL_PAGE_DIRECTORIES_INDEX != 0x3
#error "XaninPAgeDirectories index must be equal to 0x3"
#endif 

enum PAGE_DIRECTORY_ENTRY_4MB
{
    PAGE_DIRECTORY_PRESENT = 1,
    PAGE_DIRECTORY_RW = 1 << 1,
    PAGE_DIRECTORY_US = 1 << 2,
    PAGE_DIRECTORY_PWT = 1 << 3,
    PAGE_DIRECTORY_PCD = 1 << 4,
    PAGE_DIRECTORY_A = 1 << 5,
    PAGE_DIRECTORY_D = 1 << 6,
    PAGE_DIRECTORY_PS = 1 << 7,
    PAGE_DIRECTORY_G = 1 << 8
};

typedef struct {
    union {
        uint32_t fields;
        struct {
            uint8_t present : 1;
            uint8_t rw : 1;
            uint8_t us : 1;
            uint8_t pwt : 1;
            uint8_t pcd : 1;
            uint8_t accessed : 1;
            uint8_t dirty : 1;
            uint8_t page_size : 1;
            uint8_t global : 1;
            uint8_t ignored : 3;
            uint8_t pat : 1;
            uint16_t reserved : 9;
            uint16_t physical_address : 10;
        } __attribute__((packed));
    };
}__attribute__((packed)) PageDirectoryEntry4MB;


static inline bool cpu_pae_supported(void)
{
    return (cpuid(0, 1).edx & CPUID_PAE) != 0;
}

static inline bool cpu_pse36_supported(void)
{
    return (cpuid(0, 1).edx & CPUID_PSE36) != 0;
}

static inline uint32_t cpu_maxphyaddr_get(void)
{
    uint32_t eax = cpuid(0x80000008, 0).eax;

    //if this cpuid is not supported
    if (eax == 0) {
        return cpu_pae_supported() == true ? 36 : 32;
    }

    //if this cpuid is supported
    return eax & 0xFF;
}

void paging_init(void);
void page_directory_entry_set(uint32_t index, uint32_t page_base_address);
void paging_enable(void);
