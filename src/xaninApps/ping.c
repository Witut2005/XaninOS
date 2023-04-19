
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/string.h>
#include <libc/memory.h>
#include <libc/endian.h>
#include <netapi/objects/response.h>


int ping(char* ip_addr_str)
{

    if(!network_device_available_check())
    {
        xprintf("%zCANT EXECUTE PING COMMAND: NO NETWORK CARD\n", stderr);
        while(inputg().scan_code != ENTER);
        return XANIN_ERROR;
    }

    xprintf("ping: ");

    uint32_t ip_dest = str2ipv4(ip_addr_str);
    uint8_t* tmp = (uint8_t*)&ip_dest;

    for(int i = 3; i >=1; i--)
    {
        xprintf("%d.", tmp[i]);
    }

    xprintf("%d\n", tmp[0]);


    NetworkResponse* IcmpResponse;
    response_object_create(&IcmpResponse, sizeof(IcmpPacket));

    for(int i = 0; i < 4; i++)
    {
        icmp_ping(ip_dest, IcmpResponse);
        msleep(1500);

        if(IcmpResponse->success)
        {
            IcmpPacket* PacketData = IcmpResponse->data;
            xprintf("%z%s replied %d/%d\n", set_output_color(green, white),ip_addr_str, PacketData->echo_id, PacketData->echo_sequence);
            free(PacketData);
        }
        else
            xprintf("%z%s not found\n", stderr, ip_addr_str);

        

    }


    while(inputg().scan_code != ENTER);
    return XANIN_OK;


}
