
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/stdlibx.h>
#include <libc/endian.h>

void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code)
{

}

int echo_id_global = 1;
int echo_seq_global = 1;

void icmp_ping(uint32_t ip_dest)
{

    IcmpPacket* packet = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    packet->icmp_type = ICMP_ECHO_REQUEST;
    packet->icmp_code = 0x0;
    
    packet->echo_sequence = endian_switch16(echo_seq_global);
    packet->echo_id = endian_switch16(0x1);
    
    packet->checksum = 0x0;
    packet->checksum = ipv4_checksum_get((uint16_t*)packet, sizeof(IcmpPacket));

    echo_seq_global++;
    echo_id_global++;
    

    uint32_t ip_src = (192 << 24)  | (168 << 16) | ( 19 << 8) | 12;

    ipv4_packet_send(ip_dest, ip_src, INTERNET_CONTROL_MESSAGE_PROTOCOL, 128, (uint8_t*)packet, sizeof(IcmpPacket) + 0x20);


}
