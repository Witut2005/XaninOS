
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>
#include <sys/input/input.h>
#include <sys/net/network_protocols/dhcp/dhcp.h>

// TERMINAL_APP

int dhcp_test(void)
{
    while (!input_is_normal_key_pressed(KBP_F4)) // CANT BE HANDLED ASYNCHRONOUS
    {
        dhcp_discover_send();
        sleep(1); // WILL FREEZE LOOP
    }
    return XANIN_OK;
}
