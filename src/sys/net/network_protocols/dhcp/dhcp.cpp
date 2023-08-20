
#include <sys/net/network_protocols/dhcp/dhcp.h>
#include <lib/libcpp/endian.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>
#include <sys/net/netapi/network_device.h>
#include <sys/net/network_protocols/udp/udp.hpp>

void DynamicHostConfigurationProtocolSubystem::discover_send(void)
{



    DhcpHeader* Message = (DhcpHeader*)malloc(SIZE_OF(DhcpHeader));
    Message->opcode = 1;
    Message->htype = 1;
    Message->hlen = 1;
    Message->hops = 0;

    Message->xid = BIG_ENDIAN(static_cast<uint32_t>(0x3903F326));
    Message->secs = 0;
    Message->flags = 0;

    Message->client_ip_address = 0;
    Message->your_ip_address = 0;
    Message->server_ip_address = 0;
    Message->gateway_ip_address = 0;

    memcpy(Message->client_hardware_address, netapi_mac_get(xanin_ip_get()), 6);
    memset(Message->reserved, 0x0, 10);
    memset(Message->server_name, 0x0, 64);
    memset(Message->boot_filename, 0x0, 128);

    Message->magic_cookie = BIG_ENDIAN(static_cast<uint32_t>(DHCP_MAGIC_COOKIE));

    Message->options[0] = 0x35;
    Message->options[1] = 0x01;
    Message->options[2] = 0x01;

    Message->endmark = 0xFF;

    UserDatagramProtocolInterface* UdpPacket = (UserDatagramProtocolInterface*)malloc(SIZE_OF(UserDatagramProtocolInterface));
    UdpPacket->ipv4_send(UDP_BROADCAST, xanin_ip_get(), DHCP_SERVER_PORT, DHCP_CLIENT_PORT, (uint8_t*)Message, DHCP_HEADER_SIZE, NULL);

    free(Message);
    free(UdpPacket);

}

extern "C" void dhcp_discover_send(void)
{
    DynamicHostConfigurationProtocolSubystem Message;
    Message.discover_send();
}
