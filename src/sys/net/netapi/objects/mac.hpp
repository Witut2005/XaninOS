
#pragma once

#include <lib/libc/memory.h>
#include <sys/macros.h>

namespace net
{


extern bool is_system_mac(uint8_t* mac);
extern bool is_loopback_mac(uint8_t* mac);
struct MacAddress
{
    uint8_t address[6];

    MacAddress(const uint8_t* mac)
    {
        memcpy(this->address, const_cast<uint8_t*>(mac), SIZE_OF(address));
    }

    MacAddress(uint8_t* mac)
    {
        memcpy(this->address, const_cast<uint8_t*>(mac), SIZE_OF(address));
    }

    bool operator == (MacAddress other) const{
        return memcmp(const_cast<uint8_t*>(this->address), other.address, SIZE_OF(address));
    }

    bool operator != (MacAddress other) const{
        return !memcmp(const_cast<uint8_t*>(this->address), other.address, SIZE_OF(address));
    }

};

}