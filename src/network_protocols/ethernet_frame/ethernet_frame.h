
#pragma once

#include <stdint.h>

#define ETHERNET_FRAME_MAC_HEADER_SIZE 14

struct EthernetFrame
{
    uint8_t mac_destination[6];
    uint8_t mac_source[6];
    uint16_t ethernet_type;

    uint8_t* data;
    uint8_t checksum[4];

};


#ifndef __cplusplus
    extern void ethernet_frame_send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length);
#endif