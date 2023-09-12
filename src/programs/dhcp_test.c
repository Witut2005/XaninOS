
#pragma once

#include <sys/net/network_protocols/dhcp/dhcp.h>
#include <lib/libc/stdlibx.h>

//TERMINAL_APP

int dhcp_test(void)
{
    while(KeyInfo.scan_code != ENTER)
    {
        dhcp_discover_send();
        sleep(1);
    }
    return XANIN_OK;
}