#pragma once

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <lib/time.h>



void zsk_init(void)
{
    clearScr();

    Screen.x = 0;
    Screen.y = 15;

    xprintf("                                       \n");
    xprintf("              ############             \n");
    xprintf("          #############                \n");
    xprintf("        #######                        \n");
    xprintf("       ##                              \n");
    xprintf("      II    ________                   \n");
    xprintf("      II    | ----- |                  \n");
    xprintf("   ___II____| |___| |                  \n");
    xprintf("  |                 |     __________   \n");
    xprintf(" |O|                |    |          |  \n");
    xprintf("  |_________________|=||=|    %zZSK%z   |=|\n", set_output_color(lblue,white), set_output_color(black, white));
    xprintf(" /====|_____|====O |     |__________|  \n");
    xprintf("  {___}      L____/        O      O    \n");
}

void zsk_move(void)
{
    uint16_t* tmp = VGA_TEXT_MEMORY;
    
    
    if(*tmp == (uint16_t)('#' + (((black << 4) | white) << 8)))
    {
        zsk_init();
    }

    for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
    {
        tmp[i] = tmp[i + 1];
    }

    sleep5(1);

}

void zsk(void)
{

    zsk_init();

    while(getscan() != ENTER)
        zsk_move();

    app_exited = true;

}