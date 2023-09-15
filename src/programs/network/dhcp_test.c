
#include <sys/net/network_protocols/dhcp/dhcp.h>
#include <lib/libc/stdlibx.h>
#include <sys/input/input.h>
#include <lib/libc/time.h>

//TERMINAL_APP

int dhcp_test(void)
{
    while(KeyInfo.scan_code != ENTER) // CANT BE HANDLED ASYNCHRONOUS
    {
        dhcp_discover_send();
        sleep(1); // WILL FREEZE LOOP
    }
    return XANIN_OK;
}