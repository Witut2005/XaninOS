
#pragma once

#include <libc/memory.h>

namespace net

{

struct MacAddress
{
    uint8_t address[6];

    MacAddress(const uint8_t* mac)
    {
        memcpy(this->address, const_cast<uint8_t*>(mac), sizeof(address));
    }

    MacAddress(uint8_t* mac)
    {
        memcpy(this->address, const_cast<uint8_t*>(mac), sizeof(address));
    }

    bool operator == (MacAddress other) const{
        return memcmp(const_cast<uint8_t*>(this->address), other.address, sizeof(address));
    }

    bool operator != (MacAddress other) const{
        return !memcmp(const_cast<uint8_t*>(this->address), other.address, sizeof(address));
    }

};

}