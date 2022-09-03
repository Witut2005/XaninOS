
#include <network_protocols/arp/arp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/string.h>
#include <libc/memory.h>
#include <netapi/network_device.h>
#include <libc/endian.h>


int ping(char* ip_addr_str)
{
    xprintf("ping %s", ip_addr_str);
    uint32_t ip_dest = str2ipv4(ip_addr_str);
    uint8_t ip_addr[4] = {0x0};
    uint8_t macd[6] = {0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF};

    AddressResolutionProtocol* arp = (AddressResolutionProtocol*)malloc(sizeof(AddressResolutionProtocol));
    // prepare_arp_request(arp, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_GET_MAC, netapi_mac_get(), create_ip_address(ip_addr), macd, ip_dest);


    ip_dest = endian_switch32(ip_dest);
    uint8_t* tmp = (uint8_t*)&ip_dest;
        
    xprintf("\n");



    xprintf("\n\n\n\n");

    for(int i = 0; i < 4; i++)
    {

        prepare_arp_request(arp, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_GET_MAC, netapi_mac_get(), create_ip_address(ip_addr), macd, create_ip_address((uint8_t*)&ip_dest));
        send_arp_request(arp);

        if(memcmp(LastArpReply.ip_address, tmp, 4))
            xprintf("%z%s host reached\n", set_output_color(green,white), ip_addr_str);
        else
            xprintf("%zNo such host\n", set_output_color(red,white));

        ip_addr[3]++;
        msleep(1500);
    }

    free(arp);

    while(KeyInfo.scan_code != ENTER);


}