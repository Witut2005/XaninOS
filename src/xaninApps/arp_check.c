
#include <libc/string.h>
#include <libc/stdiox.h>
#include <network_protocols/arp/arp.h>

int arp_check(const char* ip_addr_str)
{

    ArpTableEntry* table = ArpTable;


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


    while(inputg().scan_code != ENTER);
    return XANIN_OK;

}
