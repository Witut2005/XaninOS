
#pragma once

#include <stdint.h>
#include <network_protocols/icmp/icmp.h>


static inline uint16_t endian_switch(uint16_t nb) 
{
    return (nb>>8) | (nb<<8);
}

static inline uint32_t endian_switch(uint32_t nb)
{
    return ((nb>>24)&0xff)      |
            ((nb<<8)&0xff0000)   |
            ((nb>>8)&0xff00)     |
            ((nb<<24)&0xff000000); 
}

static inline int16_t endian_switch(int16_t nb) 
{
    return (nb>>8) | (nb<<8);
}

static inline int32_t endian_switch(int32_t nb)
{
    return ((nb>>24)&0xff)      |
            ((nb<<8)&0xff0000)   |
            ((nb>>8)&0xff00)     |
            ((nb<<24)&0xff000000); 
}

extern IcmpPacket* endian_switch(IcmpPacket* IcmpData);

#define BIG_ENDIAN(nb) endian_switch(nb)
#define LITTLE_ENDIAN(nb) endian_switch(nb)

