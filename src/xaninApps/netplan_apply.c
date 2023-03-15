#include <libc/hal.h>
#include <xin_fs/xin.h>

int netplan_apply(void)
{
    interrupt_disable();

    for(int i = 0; i < 50; i++)
        io_wait();

    XinEntry* Netplan = fopen("/config/netplan.conf", "r");
    uint8_t* netplan_data = calloc(ARRAY_LENGTH("192.168.019.012  //XaninOS nic IP address"));
    fread(Netplan, netplan_data,ARRAY_LENGTH("192.168.019.012  //XaninOS nic IP address"));
    XinEntry* NicConfig = fopen("/config/nic.conf", "w");
    fwrite(NicConfig, netplan_data, ARRAY_LENGTH("192.168.019.012  //XaninOS nic IP address"));

    fclose(&Netplan);
    fclose(&NicConfig);

    interrupt_enable();
    return XANIN_OK;

}