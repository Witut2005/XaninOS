
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <sys/net/network_protocols/udp/udp.h>
#include <sys/input/input.h>

//TERMINAL_APP

int ip_test(char* arg)
{
    
    uint32_t ip_dest = str2ipv4(arg); 
    uint32_t ip_src = (192 << 24)  | (168 << 16) | (0 << 8) | 12;


    while(KeyInfo.scan_code != ENTER)
        ipv4_packet_send(ip_dest, ip_src, USER_DATAGRAM_PROTOCOL, 0x10, (uint8_t*)64, 0x100, NULL);

    return XANIN_OK;
}