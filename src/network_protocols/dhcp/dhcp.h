

#pragma once

#include <stdint.h>

#define DHCP_CLIENT_PORT 68
#define DHCP_SERVER_PORT 67
#define DHCP_MAGIC_COOKIE 0x63825363

#define DHCP_HEADER_SIZE 265

struct DhcpOptions
{
    // uint8_t option[3];
    // uint32_t ip


};
struct DhcpHeader
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
    uint8_t reserved[8];

    uint8_t server_name[64];
    uint8_t boot_filename[128];

    uint32_t magic_cookie;

    uint8_t options[16];
    uint8_t endmark;
    // 4+6+6
    // uint32_t aha;
    // DhcpOptions MessageOptions;


}__attribute__((packed));

#ifdef __cplusplus

class DynamicHostConfigurationProtocolSubystem
{

    public:
    void discover_send(void);

};

#else

typedef struct DhcpHeader DhcpHeader;
typedef struct DhcpOptions DhcpOptions;
extern void dhcp_discover_send(void);

#endif