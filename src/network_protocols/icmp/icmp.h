
#pragma once

#include <stdint.h>

enum ICMP_TYPE
{

    ICMP_ECHO_REPLY = 0,
    ICMP_DESTINATION_UNREACHABLE = 3

};

struct IcmpPacket
{
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t checksum;
    uint32_t rest;
}__attribute__ ((packed));



#ifndef __cplusplus
typedef struct icmp icmp;
void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code)
#else
extern "C" void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code)
#endif