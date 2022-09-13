
#pragma once

#include <stdint.h>

#define INTERNET_CONTROL_MESSAGE_PROTOCOL 0x1

enum ICMP_TYPE
{

    ICMP_ECHO_REPLY = 0,
    ICMP_DESTINATION_UNREACHABLE = 3,
    ICMP_ECHO_REQUEST = 8

};

struct IcmpPacket
{
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t checksum;
    uint32_t rest;
}__attribute__ ((packed, aligned(0x2)));



#ifndef __cplusplus
typedef struct IcmpPacket IcmpPacket;
void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code);
void icmp_ping(uint32_t ip_dest);
#else
extern "C" void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code);
#endif