
#include <network_protocols/dhcp/dhcp.h>
#include <libcpp/endian.h>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <netapi/network_device.h>
#include <network_protocols/udp/udp.hpp>

void DynamicHostConfigurationProtocolSubystem::discover_send(void)
{



    DhcpHeader* Message = (DhcpHeader*)malloc(sizeof(DhcpHeader));
    Message->opcode = 1;
    Message->htype = 1;
    Message->hlen = 1;
    Message->hops = 0;

    Message->xid = endian_switch(0x3903F326);
    Message->secs = endian_switch(0);
    Message->flags = endian_switch(0);

    Message->client_ip_address = endian_switch(0);
    Message->your_ip_address = endian_switch(0);
    Message->server_ip_address = endian_switch(0);
    Message->gateway_ip_address = endian_switch(0);

    memcpy(Message->client_hardware_address, netapi_mac_get(), 6);
    memset(Message->reserved, 0x0, 10);
    memset(Message->server_name, 0x0, 64);
    memset(Message->boot_filename, 0x0, 128);

    Message->magic_cookie = endian_switch(DHCP_MAGIC_COOKIE);

    Message->options[0] = 0x35;
    Message->options[1] = 0x01;
    Message->options[2] = 0x01;

    Message->endmark = 0xFF;

    UserDatagramProtocolInterface* UdpPacket = (UserDatagramProtocolInterface*)malloc(sizeof(UserDatagramProtocolInterface));
    UdpPacket->ipv4_send(UDP_BROADCAST, (192 << 24) | (168 << 16) | (0 << 8) | 12, DHCP_SERVER_PORT, DHCP_CLIENT_PORT, (uint8_t*)Message, DHCP_HEADER_SIZE);

    free(Message);
    free(UdpPacket);

}

extern "C" void dhcp_discover_send(void)
{
    DynamicHostConfigurationProtocolSubystem Message;
    Message.discover_send();
}
