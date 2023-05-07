
#include <sys/net/network_protocols/udp/udp.h>
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <lib/libc/endian.h>
#include <sys/input/input.h>

int udp_test(char* ip_addr_str)
{

    uint32_t ip_dest = str2ipv4(ip_addr_str);
    uint8_t src_ip[] = {192,168,19,12};

    while(KeyInfo.scan_code != ENTER)
        udp_ipv4_packet_send(endian_switch32(ip_dest), create_ip_address(src_ip), 80, 80, 200, (uint8_t*)0x0);
    
}