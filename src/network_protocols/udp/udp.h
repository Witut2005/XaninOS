
#pragma once

#include <stdint.h>

#define USER_DATAGRAM_PROTOCOL 0x11
#define UDP_HEADER_SIZE 0x8

struct UdpHeader
{

    uint16_t source_port;
    uint16_t destinantion_port;
    uint16_t length;
    uint16_t checksum;

}__attribute__((packed));