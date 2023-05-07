
#pragma once

#include <lib/libc/stdiox.h>

int execute_addr(char* address_str)
{
    uint32_t address = strtoi(address_str, HEXADECIMAL);
    void (*address_to_execute)(void) = address;
    address_to_execute();
    return XANIN_OK;
}
