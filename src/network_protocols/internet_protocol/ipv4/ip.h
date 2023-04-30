
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <libc/stdlibx.h>
#include <netapi/objects/response.h>

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

#ifdef __cplusplus
extern "C" {
#endif

extern void icmp_module_init(void);

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus

extern uint32_t create_ip_address(uint8_t ip_address[4]);
extern void ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl, uint8_t* data, uint16_t packet_size, NetworkResponse* Response);
extern uint16_t ipv4_checksum_get(uint8_t* packet_data, uint32_t data_size);
extern void ipv4_packet_receive(Ipv4Header* PacketData);
extern bool is_loopback_packet(void);

#else
#include <libcpp/map.h>
#include <libcpp/list.h>

class InternetProtocolInterface
{
    
    private:
    static NetworkResponse* Response;
    static address_t PacketSent;
    static std::UnorderedMap<std::pair<uint16_t, uint16_t>, NetworkResponse*> IcmpPacketsInfo;

    public:
    uint32_t create_ip_address(uint8_t ip_address[4]);
    void ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl, uint8_t* data, uint16_t packet_size, NetworkResponse* Response);
    void ipv4_packet_receive(Ipv4Header* PacketData);
    
    friend void icmp_module_init(void);

};


#endif


