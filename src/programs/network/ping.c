
#include <sys/net/network_protocols/icmp/icmp.h>
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <lib/libc/string.h>
#include <lib/libc/memory.h>
#include <lib/libc/endian.h>
#include <sys/net/netapi/objects/response.h>
#include <sys/input/input.h>
#include <lib/libc/stdiox.h>

//TERMIMAL_APP

int ping(char* ip_addr_str)
{

    if (!network_device_available_check())
    {
        xprintf("%zCANT EXECUTE PING COMMAND: NO NETWORK CARD\n", OUTPUT_COLOR_ERROR_SET);
        while (getxchar().scan_code != ENTER);
        return XANIN_ERROR;
    }

    xprintf("ping: ");

    uint32_t ip_dest = str2ipv4(ip_addr_str);
    uint8_t* tmp = (uint8_t*)&ip_dest;

    if (check_string_errors(IPV4_ERRNO))
    {
        xprintf("IP error (invalid ip given)\n");
        while (getxchar().scan_code != ENTER);
        return XANIN_OK;
    }

    for (int i = 3; i >= 1; i--)
        xprintf("%d.", tmp[i]);

    xprintf("%d\n\n\n", tmp[0]);

    NetworkResponse* IcmpResponse;
    response_object_create(&IcmpResponse, SIZE_OF(IcmpPacket));

    for (int i = 0; i < 4; i++)
    {
        icmp_ping(ip_dest, IcmpResponse);
        msleep(1500);

        if (IcmpResponse->success)
        {
            IcmpPacket* PacketData = (IcmpPacket*)IcmpResponse->data;
            xprintf("%z%s replied %d/%d\n", OUTPUT_COLOR_SET(green, white), ip_addr_str, PacketData->echo_id, PacketData->echo_sequence);
            free(PacketData);
        }

        else
            xprintf("%z%s not found\n", OUTPUT_COLOR_ERROR_SET, ip_addr_str);



    }
    return XANIN_OK;
}
