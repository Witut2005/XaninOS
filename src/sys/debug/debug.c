
#include <sys/debug/debug.h>
#include <lib/libc/memory.h>

XaninKernelDebugBlock XaninKernelDebugInfo;

void kernel_debug_block_set(XaninKernelDebugBlock Block)
{
    memcpy((uint8_t*)&XaninKernelDebugInfo, (uint8_t*)&Block, sizeof(XaninKernelDebugInfo));
}

XaninKernelDebugBlock kernel_debug_block_get(void)
{
    return XaninKernelDebugInfo;
}