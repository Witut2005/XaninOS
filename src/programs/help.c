
#pragma once

#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <sys/devices/keyboard/scan_codes.h>

//TERMINAL_APP

int help(char* command)
{


    if(bstrcmp(command, "help") | bstrcmp(command, ""))
    {
        xprintf("AVAILABLE COMMANDS:\n");
        xprintf("cls                 note\n");
        xprintf("create              mkdir\n");
        xprintf("pong                cd\n");
        xprintf("reg_dump            cpu_info\n");
        xprintf("load addr_in_hex    buffers\n");
        xprintf("loadch addr_in_hex  paint\n");
        xprintf("note file_name      pwd\n");
        xprintf("epilepsy            logo\n");
        xprintf("key-test            elft\n");
        xprintf("hexeditor           run16\n");
        xprintf("run                 calc\n");
        xprintf("reboot              shutdown\n");
        xprintf("move/mv             cp\n");
        xprintf("xin_info            xpaint\n");
        xprintf("ls                  nic_info\n");
        xprintf("gyn                 screenshot\n");
        xprintf("idt_examine\n");
    }

    else if(bstrcmp(command, "cls"))
    {
        xprintf("clears screen (useless)");
    }

    else if(bstrcmp(command, "note"))
    {
        xprintf("command syntax: note file_name");
        xprintf("Note app allow you to edit your files in ASCII format\n");
    }

    else if(bstrcmp(command, "create"))
    {
        xprintf("command syntax: create file_name\n");
        xprintf("Creates new file\n");
    }

    else if(bstrcmp(command, "mkdir"))
    {
        xprintf("command syntax: mkdir file_name\n");
        xprintf("creates new directory");
    }

    else if(bstrcmp(command, "pong"))
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

    else if(bstrcmp(command, "cd"))
    {
        xprintf("command syntax: cd new_directory\n");
        xprintf("changes directory\n");
    }

    else if(bstrcmp(command, "reg_dump"))
    {
        xprintf("command syntax: reg_dump\n");
        xprintf("print current values of registers\n");
    }


    else if(bstrcmp(command, "cpu_info"))
    {
        xprintf("command syntax: cpu_info\n");
        xprintf("print basic info about CPU\n");
    }

    else if(bstrcmp(command, "load"))
    {
        xprintf("command syntax: load hex_address\n");
        xprintf("Load program loads given address to the screen in hexadecimal format\n");
        xprintf("USAGE EXAMPLES\n");
        xprintf("loadch 7c00\n");
        xprintf("loadch 85f0\n");
    }

    else if(bstrcmp(command, "loadch"))
    {
        xprintf("Load program loads given address to the screen in ASCII format\n");
        xprintf("USAGE EXAMPLES\n");
        xprintf("load 7c00\n");
        xprintf("load 85f0\n");
    }

    else if(bstrcmp(command, "paint"))
    {
        xprintf("command syntax: paint file_name\n");
        xprintf("Simple paint program\n\n");
        xprintf("KEYBINDINGS\n");
        xprintf("0-f - set cell color (using bios colors)\n");
        xprintf("arrows - switch current cell\n");
        xprintf("F4 - exit\n");

    }

    else if(bstrcmp(command, "note"))
    {
        xprintf("command syntax: note file_name\n");
        xprintf("opens note text editor with given file\n");
    }

    else if(bstrcmp(command, "pwd"))
    {
        xprintf("command syntax: pwd\n");
        xprintf("print current directory name\n");
    }

    else if(bstrcmp(command, "epilepsy"))
    {
        xprintf("weird program for stickewq\n");
    }


    else if(bstrcmp(command, "logo"))
    {
        xprintf("command syntax: logo new_logo_color\n");
        xprintf("changes current logo color\n");
        xprintf("USAGE EXAMPLES\n");
        xprintf("logo white\n\n");
        xprintf("For all color attributes type 'help color'");
    }

    else if(bstrcmp(command, "key-test"))
    {
        xprintf("key-test program\n");
    }

    else if(bstrcmp(command, "elft"))
    {
        xprintf("command syntax: elft file_name\n");
        xprintf("runs executable file in ELF format\n");
    }

    else if(bstrcmp(command,"hexeditor"))
    {
        xprintf("command syntax: hexeditor file_name\n");
        xprintf("opens hexeditor with given file\n");
    }

    else if(bstrcmp(command, "run16"))
    {
        xprintf("command syntax: run16 file_name\n");
        xprintf("runs 16bit raw executable file\n");
    }

    else if(bstrcmp(command, "run32"))
    {
        xprintf("command syntax: run32 file_name\n");
        xprintf("runs 32bit raw executable file\n");
    }

    else if(bstrcmp(command, "calc"))
    {
        xprintf("calculator :))\n");
    }

    else if(bstrcmp(command, "reboot"))
    {
        xprintf("reboots computer\n");
    }

    else if (bstrcmp(command, "shutdown"))
    {
        xprintf("shutdowns computer\n");
    }

    else if(bstrcmp(command, "move/mv"))
    {
        xprintf("command syntax: move/mv src dest\n");
        xprintf("moves file to another directory\n");
    }


    else if(bstrcmp(command, "color"))
    {
        char* color_attributes[16] = {"black", "blue", "green", "cyan", "red", "magenta", "brown", "lgray", "dgray","lblue", "lgreen", "lcyan", "lred", "lmagenta", "yellow", "white"};
        xprintf("Color attributes:\n");

        for(int i = 0; i < 16; i++)
            xprintf("%d. %s\n", i, color_attributes[i]);

    }
    
    else if(bstrcmp(command, "gyn"))
    {
        xprintf("command syntax: gyn file_name\n");
        xprintf("gyn command language interpreter\n");
    }

    else if(bstrcmp(command, "nic_info"))
    {
        xprintf("prints info about network card\n");
    }

    else if(bstrcmp(command, "cp"))
    {
        xprintf("command syntax: cp src dest\n");
        xprintf("copies files\n");

    }

    else if(bstrcmp(command, "xin_info"))
    {
        xprintf("command syntax: xin_info file_name\n");
        xprintf("prints info about XinEntry\n");
    }

    else if(bstrcmp(command, "xpaint"))
    {
        xprintf("command syntax: xpaint file_name\n");
        xprintf("open xpaint editor with given file (highly expermental\n");
    }

    else if(bstrcmp(command, "ls"))
    {
        xprintf("command syntax: ls directory_name\n");
        xprintf("list all entries within given directory\n");
    }

    else if(bstrcmp(command, "screenshot"))
    {
        xprintf("makes screenshot (can be ignited with PrtSc key)");
    }

    return XANIN_OK;
}
