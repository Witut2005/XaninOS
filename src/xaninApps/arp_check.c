
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

        xprintf("%zarp entry found\n", set_output_color(green,white));

        for(int i = 0; i < 5; i++)
            xprintf("%x:", ArpTable[x].mac_address[i]);
        xprintf("%x\n", ArpTable[x].mac_address[5]);

    }

    else
        xprintf("%zno such arp table entry\n", set_output_color(red,white));


    while(inputg().scan_code != ENTER);
    return XANIN_OK;

}
