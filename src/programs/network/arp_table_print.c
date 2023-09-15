
#include <sys/net/network_protocols/arp/arp.h>
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>
#include <sys/input/input.h>

//TERMINAL_APP

extern char* argv[5]; // USE SYSCALL HERE

int arp_table_print(char* arg, char* options)
{

    uint32_t offset = 0;

    if(bstrcmp(options, "--offset"))
        offset = strtoi(argv[2], DECIMAL);

    const ArpTableEntry* table = (ArpTableEntry*)ArpTable;
    xprintf("  mac address      ip address\n\n\n");

    for(int i = offset; i < current_arp_entry; i++)
    {
        int j = 0;

        for(; j < 5; j++)
            xprintf("%mX:", table[i].mac[j]);

        xprintf("%mX", table[i].mac[5]);
        xprintf(" -> ");


        uint8_t* tmp = (uint8_t*)table[i].ip;

        for(j = 3; j > 0; j--)
            xprintf("%d.", tmp[j]);

        xprintf("%d\n", tmp[0]);
    }

    return XANIN_OK;
}
