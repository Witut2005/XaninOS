
#pragma once

#include <stdint.h>



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
