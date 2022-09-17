
#pragma once

#include <stdint.h>
#include <network_protocols/udp/udp.hpp>

#define IPV4_HEADER_VERSION 0x4

struct Ipv4Header
{

    uint8_t version_ihl;
    uint8_t tos;
    uint16_t packet_size;
    uint16_t identification;
    uint16_t fragment_offset_and_flags;
    uint8_t time_to_live;
    uint8_t protocol; 
    uint16_t checksum;
    uint32_t source_ip_address;
    uint32_t destination_ip_address;

}__attribute__((packed));

class InternetProtocolInterface
{
    
    public:
    uint32_t create_ip_address(uint8_t ip_address[4]);
    void ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size, uint8_t ttl, uint8_t* data);

};