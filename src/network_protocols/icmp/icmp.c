
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/stdlibx.h>

void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code)
{

}

void icmp_ping(uint32_t ip_dest)
{

    IcmpPacket* packet = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    packet->icmp_type = ICMP_ECHO_REQUEST;
    packet->icmp_code = 0x0;
    packet->checksum = 0x07FF;

    uint8_t ip_src[] = {192,168, 19, 12};

    ipv4_packet_send(ip_dest, create_ip_address(ip_src), INTERNET_CONTROL_MESSAGE_PROTOCOL, sizeof(IcmpPacket), (uint8_t*)packet);


}
