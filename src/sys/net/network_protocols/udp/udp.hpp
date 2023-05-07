
#pragma once

#include <stdint.h>
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>

#define USER_DATAGRAM_PROTOCOL 0x11
#define UDP_HEADER_SIZE 0x8

#define UDP_BROADCAST 0xFFFFFFFF

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
    void ipv4_send(uint32_t dest_ip, uint32_t src_ip, uint16_t dest_port, uint16_t src_port, uint8_t* data, uint16_t packet_size, NetworkResponse* Response);
};