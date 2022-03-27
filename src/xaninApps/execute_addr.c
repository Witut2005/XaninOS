
#pragma once

#include <lib/stdiox.h>

void execute_addr(uint32_t address)
{
    void (*address_to_execute)(void) = address;
    address_to_execute();
}