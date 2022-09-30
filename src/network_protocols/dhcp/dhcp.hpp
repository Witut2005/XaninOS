

#pragma once

#include <stdint.h>

struct DhcpDiscoverMessage
{
    uint8_t opcode;
    uint8_t htype;
    uint8_t hlen;
    uint8_t hops;

    uint32_t xid;

    uint16_t secs;
    uint16_t flags;

    uint32_t client_ip_address;
    uint32_t your_ip_address;
    uint32_t server_ip_address;
    uint32_t gateway_ip_address;

    uint8_t client_hardware_address[8];
    uint8_t reserved[10];
    uint8_t server_name[64];
    uint8_t boot_filename[128];
    uint8_t endmark;

}__attribute__((packed));

class DynamicHostConfigurationProtocolSubystem
{

    public:
    void discover_send(void);

};