
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>

//TERMINAL_APP

int execute_addr(char* address_str)
{
    void (*address_to_execute)(void) = (void(*)(void))(strtoi(address_str, HEXADECIMAL));
    address_to_execute();
    return XANIN_OK;
}
