
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <fs/xin.h>
#include <sys/log/syslog.h>

int dev_tools(char* program)
{

    char command[VGA_WIDTH];

    while(1)
    {
        xprintf(">");
        xscanf("%s", command);

        if(strcmp(command, "fclose"))
        {
            XinEntry* tmp = fopen("/syslog", "rw");
            fclose(&tmp);
        }

        else if(strcmp(command, "printk"))
        {
            printk("DEV TOOLS");
        }

        else if(strcmp(command, "exit"))
        {
            break;
        }

    }

    return XANIN_OK;
}