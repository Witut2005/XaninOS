
#include <network_protocols/arp/arp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/string.h>
#include <libc/memory.h>
#include <netapi/network_device.h>
#include <libc/endian.h>


int ping(char* ip_addr_str)
{

    if(!network_device_available_check())
    {
        xprintf("%zCANT EXECUTE PING COMMAND: NO NETWORK CARD\n", stderr);
        while(inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    xprintf("ping %s\n", ip_addr_str);
    uint32_t ip_dest = str2ipv4(ip_addr_str);
    uint8_t macd[6] = {0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF};

    AddressResolutionProtocol* arp = (AddressResolutionProtocol*)malloc(sizeof(AddressResolutionProtocol));
    // prepare_arp_request(arp, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_GET_MAC, netapi_mac_get(), create_ip(ip_addr), macd, ip_dest);

    uint32_t how_many_pings = 4;//strtoi(argv[2], 10) + 1;
    // xprintf("pings: %s\n", argv[2]);
    // xprintf("pings: %d\n", how_many_pings);


    // uint32_t ip_dest_big = endian_switch32(ip_dest);
    uint8_t* tmp = (uint8_t*)&ip_dest;
        
    xprintf("\n\n\n\n");

    for(int i = 0; i < how_many_pings; i++)
    {

        prepare_arp_request(arp, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_GET_MAC, netapi_mac_get(), xanin_ip_get(), macd, ip_dest);
        send_arp_request(arp);

        ArpTableEntry Response = last_arp_reply_get();

        if(Response.success)
            xprintf("%z%s host reached\n", set_output_color(green,white), ip_addr_str);
        else
            xprintf("%zNo such host\n", set_output_color(red,white));

        char* tmp = (char*)&Response.ip;
        int j;
        for(j = 3; j > 0; j--)
            xprintf("%x.", tmp[j]);

        xprintf("%x\n", tmp[0]);

        msleep(1500);
    }

    free(arp);

    while(inputg().scan_code != ENTER);
    return XANIN_OK;


}
