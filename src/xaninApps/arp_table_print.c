
#include <network_protocols/arp/arp.h>

int arp_table_print(char* arg)
{
    const ArpTableEntry* table = (ArpTableEntry*)ArpTable;
    
    // for(int i = 0; i <= sizeof(ArpTable) / sizeof(ArpTable[0]); i++)
    for(int i = 0; i < current_arp_entry; i++)
    {
        // if(i == sizeof(ArpTable) / sizeof(ArpTable[0]))
        //     return 3;

        // if(table[i].ip_address[i] != 0x0 && *(uint32_t*)&table[i].mac_address != 0x0)
        {
            int j = 0;

            xprintf("%d: ", i);

            for(; j < 5; j++)
                xprintf("%mX:", table[i].mac_address[j]);

            xprintf("%mX", table[i].mac_address[5]);
            xprintf(" -> ");


            uint8_t* tmp = (uint8_t*)table[i].ip_address;

            for(j = 3; j > 0; j--)
                xprintf("%d.", tmp[j]);

            xprintf("%d\n", tmp[0]);
        }
    }


    while(inputg().scan_code != ENTER);

}
