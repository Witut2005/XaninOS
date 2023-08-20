
#include <stdbool.h>
#include <stdint.h>
#include <sys/macros.h>

struct XaninKernelDebugBlock
{
    // XinFs 
    bool xin_fs_fopen_observe;
    bool xin_fs_close_observe;

    // Memory Allocator
    bool pmmngr_alloc_observe;
    bool pmmngr_calloc_observe;
    bool pmmngr_realloc_observe;
    bool pmmngr_free_observe;

};

typedef struct XaninKernelDebugBlock XaninKernelDebugBlock;

extern void kernel_debug_block_set(XaninKernelDebugBlock Block);
extern XaninKernelDebugBlock kernel_debug_block_get(void);