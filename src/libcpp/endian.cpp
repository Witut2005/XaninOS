

#include <stdint.h>



uint16_t endian_switch(uint16_t nb) {
    return (nb>>8) | (nb<<8);
}

uint32_t endian_switch(uint32_t nb) {
    return ((nb>>24)&0xff)      |
            ((nb<<8)&0xff0000)   |
            ((nb>>8)&0xff00)     |
            ((nb<<24)&0xff000000); }