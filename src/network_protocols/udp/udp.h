
#pragma once

#include <stdint.h>

#define USER_DATAGRAM_PROTOCOL 0x11

struct UdpHeader
{

    uint16_t source_port;
    uint16_t destinantion_port;
    uint16_t length;
    uint16_t checksum;
    uint8_t* data;

};