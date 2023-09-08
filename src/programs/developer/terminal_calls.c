
#include <sys/call/xanin_sys/calls/terminal/terminal.h>

__STATUS terminal_calls_test(void)
{
    Xtf* la = __sys_xtf_init(0x69696);
    xprintf("xtf address: 0x%x\n", la);
    xprintf("xtf 0x%x\n", la->size_allocated);
}