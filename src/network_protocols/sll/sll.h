

#pragma once

#include <stdint.h>

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
void sll_frame_send(uint16_t packet_type, uint16_t link_layer_address_type, uint16_t link_layer_address_length,
                        uint8_t mac_address[6], uint16_t protocol, uint8_t* data_to_send, uint16_t packet_size);
#ifdef __cplusplus
}
#endif