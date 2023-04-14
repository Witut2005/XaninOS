
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/stdlibx.h>
#include <libc/endian.h>

int echo_id_global = 45;
int echo_seq_global = 0;

void icmp_ping(uint32_t ip_dest)
{

    IcmpPacket* packet = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    packet->icmp_type = ICMP_ECHO_REQUEST;
    packet->icmp_code = 0x0;
    
    packet->echo_id = endian_switch16(echo_id_global);
    packet->echo_sequence = endian_switch16(echo_seq_global);
    
    packet->checksum = 0x0;
    packet->checksum = ipv4_checksum_get((uint16_t*)packet, sizeof(IcmpPacket));

    echo_seq_global++;
    echo_id_global++;
    

    uint32_t ip_src = (192 << 24)  | (168 << 16) | ( 19 << 8) | 12;

    ipv4_packet_send(ip_dest, ip_src, INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)packet, sizeof(IcmpPacket));
    free(packet);


}

void icmp_ping_reply(IcmpPacket* Packet, uint32_t ip_dest)
{
    IcmpPacket* ReplyPacket = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    ReplyPacket->icmp_type = ICMP_ECHO_REPLY;
    ReplyPacket->icmp_code = 0x0;
    
    ReplyPacket->echo_id = Packet->echo_id;
    ReplyPacket->echo_sequence = Packet->echo_sequence;
    
    ReplyPacket->checksum = 0x0;
    ReplyPacket->checksum = ipv4_checksum_get((uint16_t*)ReplyPacket, sizeof(IcmpPacket));

    uint32_t ip_src = (192 << 24)  | (168 << 16) | ( 19 << 8) | 12;

    if(is_loopback_packet())
        ipv4_packet_send(LOOPBACK_IP_ADDRESS, LOOPBACK_IP_ADDRESS, INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)ReplyPacket, sizeof(IcmpPacket));

    else
        ipv4_packet_send(ip_dest, ip_src, INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)ReplyPacket, sizeof(IcmpPacket));

    free(ReplyPacket);
}