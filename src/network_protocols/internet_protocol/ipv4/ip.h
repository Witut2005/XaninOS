
#pragma once

#include <stdint.h>


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

extern uint32_t create_ip_address(uint8_t ip_address[4]);
extern void ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size, uint8_t* data);
extern uint16_t ipv4_checksum_get(uint16_t* data, uint32_t data_size);

