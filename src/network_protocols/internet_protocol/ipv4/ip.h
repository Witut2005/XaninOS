
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define IPV4_HEADER_VERSION 0x4
#define LOOPBACK_IP_ADDRESS ((127 << 24) | (1))


struct Ipv4Header
{

    uint8_t version_ihl;
    uint8_t tos;
    uint16_t packet_size ;
    uint16_t identification;
    uint16_t fragment_offset_and_flags;
    uint8_t time_to_live;
    uint8_t protocol; 
    uint16_t checksum;
    uint32_t source_ip_address;
    uint32_t destination_ip_address;

}__attribute__((packed));

typedef struct Ipv4Header Ipv4Header;

#ifndef __cplusplus

extern uint32_t create_ip_address(uint8_t ip_address[4]);
extern void ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl, uint8_t* data, uint16_t packet_size);
extern uint16_t ipv4_checksum_get(uint16_t* data, uint32_t data_size);
extern void ipv4_packet_receive(Ipv4Header* PacketData);
extern bool is_loopback_packet(void);

#else

class InternetProtocolInterface
{
    
    public:
    uint32_t create_ip_address(uint8_t ip_address[4]);
    void ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl, uint8_t* data, uint16_t packet_size);
    void ipv4_packet_receive(Ipv4Header* PacketData);

};

#endif


