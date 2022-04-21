
#pragma once

#include <lib/stdiox.h>

void help(void)
{

    KeyInfo.scan_code = 0x0;
    
    xprintf("AVAILABLE COMMANDS:\n");
    xprintf("cls\n");
    xprintf("note file_name\n");
    xprintf("create parent_directory\n");
    xprintf("mkdir parent_directory\n");
    xprintf("pong\n");
    xprintf("cd directory_name\n");
    xprintf("reg_dump\n");
    xprintf("cpu_info");
    xprintf("load addr_in_hex\n");
    xprintf("loadch addr_in_hex\n");
    xprintf("paint file_name\n");
    xprintf("note file_name\n");
    xprintf("pwd\n");
    xprintf("epilepsy\n");
    xprintf("logo\n");
    xprintf("key-test\n");

    xscanf("%d",app_exited);

    app_exited = true;
      

}
