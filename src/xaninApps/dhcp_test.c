
#pragma once

#include <network_protocols/dhcp/dhcp.h>
#include <libc/stdlibx.h>

int dhcp_test(void)
{
    while(KeyInfo.scan_code != ENTER)
    {
        dhcp_discover_send();
        sleep(1);
    }

}