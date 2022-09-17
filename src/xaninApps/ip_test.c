
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <network_protocols/udp/udp.h>



int ip_test(char* arg)
{
    uint8_t dest_ip[] = {192,168,0,160};
    uint8_t src_ip[] = {192,168,19,12};

    while(KeyInfo.scan_code != ENTER)
        ipv4_packet_send(create_ip_address(dest_ip), create_ip_address(src_ip), USER_DATAGRAM_PROTOCOL, 0x10, 64, (uint8_t*)0x100);
}
