
#include <network_protocols/dhcp/dhcp.hpp>
#include <libcpp/endian.h>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <netapi/network_device.hpp>
#include <network_protocols/udp/udp.hpp>

void DynamicHostConfigurationProtocolSubystem::discover_send(void)
{


    DhcpDiscoverMessage* Message = (DhcpDiscoverMessage*)calloc(sizeof(DhcpDiscoverMessage));
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
    memset(Message->reserved, endian_switch(63825363), 10);
    memset(Message->server_name, 0x0, 64);
    memset(Message->boot_filename, 0x0, 128);

    Message->endmark = 0xFF;

    UserDatagramProtocolInterface* UdpPacket = (UserDatagramProtocolInterface*)malloc(sizeof(UserDatagramProtocolInterface));
    // UdpPacket->ipv4_send()


}
