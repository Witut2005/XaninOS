
#include <libc/string.h>
#include <libc/stdiox.h>
#include <network_protocols/arp/arp.h>

int arp_check(const char* ip_addr_str)
{

    uint32_t ip = str2ipv4(ip_addr_str);

    int x = mac_get_from_ip(endian_switch32(ip));

    if(x != 0xFF)
    {

        for(int i = 0; i < 5; i++)
            xprintf("%x:", ArpTable[x].mac_address[i]);
        xprintf("%x\n", ArpTable[x].mac_address[5]);

    }

    else
        xprintf("no such arp table entry\n");


    while(KeyInfo.scan_code != ENTER);

}