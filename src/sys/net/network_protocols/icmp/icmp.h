
#pragma once

#include <stdint.h>
#include <sys/net/netapi/network_device.h>
#include <sys/net/netapi/objects/response.h>
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>

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
}__attribute__ ((packed));



#ifndef __cplusplus
typedef struct IcmpPacket IcmpPacket;
#endif

#ifdef __cplusplus 
extern "C"{
#endif

void icmp_module_init(void);
void icmp_ping(uint32_t ip_dest, NetworkResponse* Response);
void icmp_ping_reply(IcmpPacket* packet, uint32_t ip_dest);
void icmp_packet_receive(IcmpPacket* Packet, uint32_t ip_src);

#ifdef __cplusplus 
}
#endif


#ifdef __cplusplus

class IcmpModule
{
    // private: 
    static std::UnorderedMap<std::pair<uint16_t, uint16_t>, NetworkResponse*> PacketsInfo;
    static uint32_t echo_id_global;
    static uint32_t echo_seq_global;

    public:
    static void ping(uint32_t ip_dest, NetworkResponse* Response);
    static void ping_reply(IcmpPacket* PacketReceived, uint32_t ip_dest);
    static void receive(IcmpPacket* IcmpPacketReceived, uint32_t ip_src);

    friend void icmp_module_init(void);
    friend class InternetProtocolInterface;
};


#endif