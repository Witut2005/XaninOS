
#include <network_protocols/arp/arp.h>
#include <network_protocols/internet_protocol/ip.h>
#include <libc/string.h>
#include <libc/memory.h>
#include <netapi/network_device.h>


int ping(char* ip_addr_str)
{
    xprintf("ping %s", ip_addr_str);
    uint32_t ip_dest = str2ipv4(ip_addr_str);
    uint8_t ip_addr[4] = {0x0};
    uint8_t macd[6] = {0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF};

    AddressResolutionProtocol* arp = (AddressResolutionProtocol*)malloc(sizeof(AddressResolutionProtocol));
    // prepare_arp_request(arp, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_GET_MAC, netapi_mac_get(), create_ip_address(ip_addr), macd, ip_dest);
    prepare_arp_request(arp, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_GET_MAC, netapi_mac_get(), create_ip_address(ip_addr), macd, create_ip_address(ip_dest));
    xprintf("gooo");
    while(KeyInfo.scan_code != ENTER);
    send_arp_request(arp);

    // for(int i = 0; i < 4; i++)
    {
        // xprintf("b");
        // xprintf("c");
        // if(memcmp(LastArpReply.ip_address, ip_dest, 4))
        //     xprintf("%zI didnt expected that this host exists lol\n", set_output_color(green,white));
        // sleep(1);
    }


}