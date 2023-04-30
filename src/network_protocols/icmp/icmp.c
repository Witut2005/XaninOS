
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/stdlibx.h>
#include <libc/endian.h>
#include <libc/memory.h>

int echo_id_global = 45;
int echo_seq_global = 0;

NetworkResponse* IcmpResponse;


void icmp_ping(uint32_t ip_dest, NetworkResponse* Response)
{

    IcmpResponse = Response;

    IcmpPacket* RequestPacket = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    RequestPacket->type = ICMP_ECHO_REQUEST;
    RequestPacket->code = 0x0;
    
    RequestPacket->echo_id = endian_switch16(echo_id_global);
    RequestPacket->echo_sequence = endian_switch16(echo_seq_global);
    
    RequestPacket->checksum = 0x0;
    RequestPacket->checksum = ipv4_checksum_get((address_t)RequestPacket, sizeof(IcmpPacket));

    echo_seq_global++;
    echo_id_global++;
    

    ipv4_packet_send(ip_dest, xanin_ip_get(), INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)RequestPacket, sizeof(IcmpPacket), Response);
    free(RequestPacket);


}

void icmp_ping_reply(IcmpPacket* Packet, uint32_t ip_dest)
{
    IcmpPacket* ReplyPacket = (IcmpPacket*)malloc(sizeof(IcmpPacket));
    ReplyPacket->type = ICMP_ECHO_REPLY;
    ReplyPacket->code = 0x0;
    
    ReplyPacket->echo_id = Packet->echo_id;
    ReplyPacket->echo_sequence = Packet->echo_sequence;
    
    ReplyPacket->checksum = 0x0;
    ReplyPacket->checksum = ipv4_checksum_get((address_t)ReplyPacket, sizeof(IcmpPacket));

    if(is_loopback_packet())
        ipv4_packet_send(LOOPBACK_IP_ADDRESS, LOOPBACK_IP_ADDRESS, INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)ReplyPacket, sizeof(IcmpPacket), NULL);

    else
        ipv4_packet_send(ip_dest, xanin_ip_get(), INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)ReplyPacket, sizeof(IcmpPacket), NULL);
    

    free(ReplyPacket);
}