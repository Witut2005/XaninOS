
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <fs/xin.h>
#include <sys/log/syslog.h>
#include <sys/debug/debug.h>
#include <lib/libc/memory.h>
#include <lib/libc/file.h>

#define DEV_TOOLS_MAX_OPTIONS_NUMBER 3

int dev_tools(char* program)
{

    XaninKernelDebugBlock DebugInfo;
    memset((uint8_t*)&DebugInfo, false, sizeof(DebugInfo));

    kernel_debug_block_set(DebugInfo);

    char* command = (char*)calloc(VGA_WIDTH);
    char* param = (char*)calloc(VGA_WIDTH);

    char* xanin_modules[] = {
        "fopen",
        "fclose",
        "malloc",
        "calloc",
        "realloc",
        "free"
    };

    while(1)
    {
        xprintf(">");
        xscanf("%s %s", command, param);

        if(strcmp(command, "enable"))
        {
            xprintf("which module of XaninOS would you like to observe:\n\n");

            for(int i = 0; i < 6; i++)
                xprintf("%s\n", xanin_modules[i]);

            char* option = (char*)calloc(VGA_WIDTH);
            xscanf("%s", option);

            for(int i = 0; i < 6; i++)
            {
                if(strcmp(option, xanin_modules[i]))
                    memset((uint8_t*)(&DebugInfo) + i, true, 1);
            }

            kernel_debug_block_set(DebugInfo);
            
        }

        else if(strcmp(command, "disable"))
        {
            xprintf("which module of XaninOS would you like not to observe:\n\n");

            for(int i = 0; i < 6; i++)
                xprintf("%s\n", xanin_modules[i]);

            char* option = (char*)calloc(VGA_WIDTH);
            xscanf("%s", option);

            for(int i = 0; i < 6; i++)
            {
                if(strcmp(option, xanin_modules[i]))
                    memset((uint8_t*)(&DebugInfo) + i, false, 1);
            }

            kernel_debug_block_set(DebugInfo);
        }

        else if(strcmp(command, "fclose"))
        {
            XinEntry* tmp = fopen("/syslog", "rw");
            fclose(&tmp);
        }

        else if(strcmp(command, "printk"))
        {
            printk(param);
        }

        else if(strcmp(command, "read"))
        {
            char buf[10] = {0};
            fread(fopen("/syslog", "r"), buf, 9);
            xprintf("%s\n", buf);
        }

        else if(strcmp(command, "exit"))
        {
            break;
        }

    }

    return XANIN_OK;
}