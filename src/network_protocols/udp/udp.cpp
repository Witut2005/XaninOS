
#include <network_protocols/udp/udp.hpp>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>
#include <libc/stdiox.h>
#include <network_protocols/internet_protocol/ipv4/ip.hpp>

/* everything is zero !!! */

void UserDatagramProtocolInterface::ipv4_send(uint32_t dest_ip, uint32_t src_ip, uint16_t dest_port, uint16_t src_port, uint8_t* data, uint16_t packet_size)
{
    UdpHeader* UdpPacket = (UdpHeader*)malloc(sizeof(1518));

    UdpPacket->destination_port = endian_switch(dest_port);
    UdpPacket->source_port = endian_switch(src_port);
    UdpPacket->packet_size = endian_switch(packet_size);
    UdpPacket->checksum = 0x0;

    memcpy((uint8_t*)UdpPacket+8, data, packet_size);

    InternetProtocolInterface* IpPacket = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
    IpPacket->ip4_packet_send(dest_ip, src_ip, USER_DATAGRAM_PROTOCOL, 0xFF, (uint8_t*)UdpPacket, packet_size);
    free(UdpPacket);
    free(IpPacket);
}


extern "C"
{

    void udp_ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint16_t dest_port, uint16_t src_port, uint8_t* data, uint16_t packet_size)
    {
        UserDatagramProtocolInterface* UdpPacket = (UserDatagramProtocolInterface*)malloc(sizeof(UserDatagramProtocolInterface));
        UdpPacket->ipv4_send(dest_ip, src_ip, dest_port, src_port, data, packet_size);
        free(UdpPacket);
    }

}