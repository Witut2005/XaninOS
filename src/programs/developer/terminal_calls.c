
#include <sys/call/xanin_sys/calls/terminal/terminal.h>

__STATUS terminal_calls_test(void)
{
    // xprintf("xtf address: 0x%x\n", la);
    // xprintf("xtf 0x%x\n", la->size_allocated);

    __sys_xtf_cell_put(vty_get(), 'n', 0x41);
    
}