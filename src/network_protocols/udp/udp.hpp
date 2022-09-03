
#pragma once

#include <stdint.h>

#define USER_DATAGRAM_PROTOCOL 0x11
#define UDP_HEADER_SIZE 0x8

struct UdpHeader
{

    uint16_t source_port;
    uint16_t destination_port;
    uint16_t packet_size;
    uint16_t checksum;

}__attribute__((packed));

class UserDatagramProtocolInterface
{
    public:
    void ipv4_send(uint32_t dest_ip, uint32_t src_ip, uint16_t dest_port, uint16_t src_port, uint16_t packet_size, uint8_t* data);
};