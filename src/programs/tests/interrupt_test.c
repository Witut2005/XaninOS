
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>

//TERMINAL_APP

int interrupt_test(char* int_id)
{
    int id = strtoi(int_id, 10);

    uint8_t code_to_execute[2] = {0xCD};
    code_to_execute[1] = id;

    void (*ptr)(void) = (void(*)(void))code_to_execute;
    ptr();

    return XANIN_OK;
}