
#pragma once

#include <libc/stdiox.h>

void help(char* command)
{

    screen_clear();

    if(strcmp(command, "help"))
    {
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
    }

    else if(strcmp(command, ""))
    {
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
    }

    else if(strcmp(command, "note"))
    {
        xprintf("command syntax: note file_name");
        xprintf("Note app allow you to edit your files in ASCII format\n");
    }

    else if(strcmp(command, "create"))
    {
        xprintf("command syntax: create file_name\n");
        xprintf("Creates new file\n");
    }

    else if(strcmp(command, "mkdir"))
    {
        xprintf("command syntax: mkdir file_name\n");
        xprintf("creates new directory");
    }

    else if(strcmp(command, "pong"))
    {
        xprintf("command syntax: pong\n");
        xprintf("simple pong game\n\n");
        xprintf("KEYBINDINGS:\n\n");

        xprintf("EXIT: ESC\n\n");

        xprintf("PLAYER 1:\n");
        xprintf("w - up\n");
        xprintf("s - down\n\n");

        xprintf("PLAYER 2:\n");
        xprintf("p - up\n");
        xprintf("l - down\n");

    }

    else if(strcmp(command, "reg_dump"))
    {
        xprintf("command syntax: reg_dump\n");
        xprintf("print current values of registers\n");
    }


    else if(strcmp(command, "cpu_info"))
    {
        xprintf("command syntax: cpu_info\n");
        xprintf("print basic info about CPU\n");
    }

    else if(strcmp(command, "load"))
    {
        xprintf("command syntax: load hex_address\n");
        xprintf("Load program loads given address to the screen in hexadecimal format\n");
        xprintf("USAGE EXAMPLES\n");
        xprintf("loadch 7c00\n");
        xprintf("loadch 85f0\n");
    }

    else if(strcmp(command, "loadch"))
    {
        xprintf("Load program loads given address to the screen in ASCII format\n");
        xprintf("USAGE EXAMPLES\n");
        xprintf("load 7c00\n");
        xprintf("load 85f0\n");
    }

    else if(strcmp(command, "paint"))
    {
        xprintf("command syntax: paint file_name\n");
        xprintf("Simple paint program\n\n");
        xprintf("KEYBINDINGS\n");
        xprintf("0-f - set cell color (using bios colors)\n");
        xprintf("arrows - switch current cell\n");
        xprintf("F4 - exit\n");

    }

    else if(strcmp(command, "pwd"))
    {
        xprintf("command syntax: pwd\n");
        xprintf("print current directory name\n");
    }



    else if(strcmp(command, "logo"))
    {
        xprintf("command syntax: logo new_logo_color\n");
        xprintf("changes current logo color\n");
        xprintf("USAGE EXAMPLES\n");
        xprintf("logo white\n\n");
        xprintf("For all color attributes type 'help color'");
    }


    else if(strcmp(command, "color"))
    {
        xprintf("Color attributes:\n");

        for(int i = 0; i < 16; i++)
            xprintf("%d. %s\n", i, color_attributes[i]);

    }
    

    while(KeyInfo.scan_code != ENTER);

}
