
#include <network_protocols/arp/arp.h>

int arp_table_print(char* arg)
{
    ArpTableEntry* table = (ArpTableEntry*)ArpTable;
    
    while(table->ip_address[0] == 0x0)
        table++;

    for(int i = 0; i <= 10; i++)
    {
        if(i == 10)
            return 3;

        if(table->ip_address[i] != 0x0 && *(uint32_t*)&table->mac_address[i] != 0x0)
            break;
    }

    int i = 0;

    for(; i < 5; i++)
        xprintf("%mX:", table->mac_address[i]);

    xprintf("%mX", table->mac_address[5]);
    xprintf(" -> ");


    uint8_t* tmp = (uint8_t*)table->ip_address;

    for(i = 0; i < 3; i++)
        xprintf("%d.", tmp[i]);

    xprintf("%d", tmp[3]);
    while(KeyInfo.scan_code != ENTER);

}
