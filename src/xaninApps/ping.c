
#include <network_protocols/icmp/icmp.h>
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

    NetworkResponse IcmpResponse;

    for(int i = 0; i < 4; i++)
    {
        icmp_ping(ip_dest, &IcmpResponse);
        msleep(1500);

        if(IcmpResponse.success)
        {
            IcmpPacket* PacketData = IcmpResponse.data;
            xprintf("%z%s replied %d/%d\n", set_output_color(green, white),ip_addr_str, PacketData->echo_id, PacketData->echo_sequence);
            free(PacketData);
        }
        else
            xprintf("%z%s not found\n", stderr, ip_addr_str);

        

    }


    while(inputg().scan_code != ENTER);
    return XANIN_OK;


}
