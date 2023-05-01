

#include <stdint.h>
#include <network_protocols/icmp/icmp.h>

uint16_t endian_switch(uint16_t nb) 
{
    return (nb>>8) | (nb<<8);
}

uint32_t endian_switch(uint32_t nb)
{
    return ((nb>>24)&0xff)      |
            ((nb<<8)&0xff0000)   |
            ((nb>>8)&0xff00)     |
            ((nb<<24)&0xff000000); 
}

int16_t endian_switch(int16_t nb) 
{
    return (nb>>8) | (nb<<8);
}

int32_t endian_switch(int32_t nb)
{
    return ((nb>>24)&0xff)      |
            ((nb<<8)&0xff0000)   |
            ((nb>>8)&0xff00)     |
            ((nb<<24)&0xff000000); 
}

IcmpPacket* endian_switch(IcmpPacket* IcmpData)
{
    IcmpData->checksum = endian_switch(IcmpData->checksum);
    IcmpData->echo_id = endian_switch(IcmpData->echo_id);
    IcmpData->echo_sequence = endian_switch(IcmpData->echo_sequence);
    return IcmpData;
}
