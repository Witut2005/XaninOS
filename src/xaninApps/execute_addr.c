
#pragma once

#include <libc/stdiox.h>

int execute_addr(char* address_str)
{
    uint32_t address = strtoi(address_str, 16);
    void (*address_to_execute)(void) = address;
    address_to_execute();
    return XANIN_OK;
}
