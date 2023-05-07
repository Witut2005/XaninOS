
#pragma once

#include <stdint.h>

#define LITTLE_ENDIAN16(nb) endian_switch16(nb) 
#define LITTLE_ENDIAN32(nb) endian_switch32(nb) 

static inline uint16_t endian_switch16(uint16_t nb)
{
    return (((nb & 0x00FF) << 8) |
            ((nb & 0xFF00) >> 8));
}

static inline uint32_t endian_switch32(uint32_t nb)
{
    return (((nb & 0x000000FF) << 24) |
            ((nb & 0x0000FF00) <<  8) |
            ((nb & 0x00FF0000) >>  8) |
            ((nb & 0xFF000000) >> 24));

}


