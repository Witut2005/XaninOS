

#include <stdint.h>

#define ETHERNET_FRAME_HEADER_SIZE 14

struct EthernetFrame
{
    uint8_t mac_destination[6];
    uint8_t mac_source[6];

    uint16_t ethernet_type;
    uint8_t* data;
    uint32_t checksum;

};