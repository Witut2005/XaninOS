
#include <libc/string.h>
#include <libc/stdlibx.h>
#include <network_protocols/icmp/icmp.h>

int icmp_test(char* ip_dest_str)
{
    uint32_t ip_dest = str2ipv4(ip_dest_str);

    while(KeyInfo.scan_code != ENTER)   
    {
        icmp_ping(ip_dest);
        msleep(4000);
    }

}