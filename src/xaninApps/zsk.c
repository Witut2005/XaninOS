#pragma once

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <lib/time.h>

void zsk_move(void)
{
    uint16_t* tmp = VGA_TEXT_MEMORY;
    
    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
    {
        tmp[i] = tmp[i + 1];
    }

    sleep5(1);

}

void zsk(void)
{

    clearScr();

    Screen.x = 0;
    Screen.y = 17;

    xprintf("                                       \n");
    xprintf("        #######                        \n");
    xprintf("       ##                              \n");
    xprintf("      II    ________                   \n");
    xprintf("      II    | ----- |                  \n");
    xprintf("   ___II____| |___| |                  \n");
    xprintf("  |                 |     __________   \n");
    xprintf(" |O|                |    |          |  \n");
    xprintf("  |_________________|=||=|   ZSK    |=|\n");
    xprintf(" /====|_____|====O |     |__________|  \n");
    xprintf("  {___}      L____/        O      O    \n");



    while(getscan() != ENTER)
        zsk_move();

}