

#pragma once

#include <stdint.h>
#include <netapi/network_device.h>
#include <libc/endian.h>

#ifndef __cplusplus
#include <libc/memory.h>
#include <libc/stdlibx.h>
#endif

struct SllFrame
{
    uint16_t packet_type;
    uint16_t link_layer_address_type;
    uint16_t link_layer_address_length;
    uint8_t mac_address[6];
    uint16_t unused;
    uint16_t protocol;
};

#ifndef __cplusplus
typedef struct SllFrame SllFrame;
#endif


#ifdef __cplusplus
extern "C" { 
#endif
static inline void sll_frame_send(uint16_t packet_type, uint16_t link_layer_address_type, uint16_t link_layer_address_length,
                        uint8_t* mac_address, uint16_t protocol, uint8_t* data_to_send, uint16_t packet_size)
{
    SllFrame* PacketFrame = (SllFrame*)calloc(packet_size);//chyba
    PacketFrame->packet_type = endian_switch16(packet_type);
    PacketFrame->link_layer_address_type = endian_switch16(link_layer_address_type);
    PacketFrame->link_layer_address_length = endian_switch16(link_layer_address_length);
    memcpy(PacketFrame->mac_address, mac_address, 6);
    PacketFrame->unused = 0x0;
    PacketFrame->protocol = endian_switch16(protocol);

    uint8_t* data = (uint8_t*)PacketFrame;
    data = data + 0x10;
    memcpy(data, data_to_send, packet_size + 0x10);
    netapi_packet_send((uint8_t*)PacketFrame, packet_size + 0x10);
}

#ifdef __cplusplus
}
#endif
