
#pragma once

#include <stdint.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>

#define INTERNET_CONTROL_MESSAGE_PROTOCOL 0x1

enum type
{

    ICMP_ECHO_REPLY = 0,
    ICMP_DESTINATION_UNREACHABLE = 3,
    ICMP_ECHO_REQUEST = 8

};

struct IcmpPacket
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;

    union
    {    

        struct
        {
            uint16_t echo_id;
            uint16_t echo_sequence;
        }__attribute__((packed));
        
        uint32_t rest;
    };
}__attribute__ ((packed, aligned(0x2)));



#ifndef __cplusplus
typedef struct IcmpPacket IcmpPacket;
#endif

#ifdef __cplusplus 
extern "C"{
#endif

void icmp_module_init(void);
void icmp_ping(uint32_t ip_dest, IcmpPacket* Response);
void icmp_ping_reply(IcmpPacket* packet, uint32_t ip_dest);

#ifdef __cplusplus 
}
#endif