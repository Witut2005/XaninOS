
#pragma once

#include <stdint.h>

#define ARP_ETHERNET 0x1
#define ARP_REQUEST_IP 0x800
#define ARP_REQUEST_REPLY 0x2

struct arp
{
    uint16_t hardware_type; // Hardware type
    uint16_t protocol_type; // Protocol type
    uint8_t hardware_address_length; // Hardware address length (Ethernet = 6)
    uint8_t protocol_address_length; // Protocol address length (IPv4 = 4)
    uint16_t opcode; // ARP Operation Code
    uint8_t source_hardware_address[6]; // Source hardware address - hlen bytes (see above)
    uint32_t source_protocol_address; // Source protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
    uint8_t destination_hardware_address[6]; // Destination hardware address - hlen bytes (see above)
    uint32_t destination_protocol_address; // Destination protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
};

#ifndef __cplusplus
    typedef struct arp arp;
#endif

