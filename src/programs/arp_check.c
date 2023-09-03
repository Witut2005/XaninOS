
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <sys/net/network_protocols/arp/arp.h>

int arp_check(const char* ip_addr_str)
{

    uint32_t ip = str2ipv4(ip_addr_str);
    int x = mac_get_from_ip(ip);

    if(x != 0xFF)
    {

        xprintf("%zarp entry found\n", OUTPUT_COLOR_SET(green,white));

        for(int i = 0; i < 5; i++)
            xprintf("%x:", ArpTable[x].mac[i]);
        xprintf("%x\n", ArpTable[x].mac[5]);

    }

    else
        xprintf("%zno such arp table entry\n", OUTPUT_COLOR_SET(red,white));


    return XANIN_OK;
}
