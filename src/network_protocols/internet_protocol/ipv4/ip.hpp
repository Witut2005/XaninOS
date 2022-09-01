
#pragma once

#include <stdint.h>
#include <network_protocols/udp/udp.h>

#define IPV4_HEADER_VERSION 0x4

struct Ipv4Header
{

    uint8_t ihl : 4;
    uint8_t version : 4;

    uint8_t dscp : 6;
    uint8_t ecn : 2;
    uint16_t packet_size;
    uint16_t identification;
    uint8_t flags : 3;
    uint16_t fragment_offset : 13;
    uint8_t time_to_live;
    uint8_t protocol; 
    uint16_t checksum;
    uint32_t source_ip_address;
    uint32_t destination_ip_address;

}__attribute__((packed));

class InternetProtocolInterface
{
    private:

    public:
    uint32_t create_ip_address(uint8_t ip_address[4]);
    void ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size);

};