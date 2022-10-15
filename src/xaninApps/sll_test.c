
#include <stdint.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <network_protocols/icmp/icmp.h>


int sll_test(char* dest_ip_str)
{
    uint32_t dest_ip = str2ipv4(dest_ip_str);

    
    // while(KeyInfo.scan_code != ENTER)
    {
        ipv4_packet_send(dest_ip, 192 << 24 | 168 << 16 | 19 << 8 | 16, INTERNET_CONTROL_MESSAGE_PROTOCOL, 128, (uint8_t*)0x0, 64 - 12);
        return XANIN_OK;
        // msleep(200);
    }
}