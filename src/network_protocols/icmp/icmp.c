
#include <network_protocols/icmp/icmp.h>
#include <libc/stdlibx.h>

void icmp_message_send(uint8_t icmp_type, uint8_t icmp_code)
{

}

void icmp_ping(uint32_t ip_dest)
{

    IcmpPacket* packet = (IcmpPacket*)malloc(sizeof(IcmpPacket));

}