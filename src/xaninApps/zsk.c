#pragma once

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <lib/time.h>



void zsk_init(void)
{
    screen_clear();

    Screen.x = 0;
    Screen.y = 15;

    xprintf("                                                               \n");
    xprintf("              ############                                     \n");
    xprintf("          #############                                        \n");
    xprintf("        #######                                                \n");
    xprintf("       ##                                                      \n");
    xprintf("      II    ________                                           \n");
    xprintf("      II    | ----- |                                          \n");   
    xprintf("   ___II____| |___| |                                          \n");
    xprintf("  |                 |     __________      __________           \n");
    xprintf(" |O|                |    |          |    |          |          \n");
    xprintf("  |_________________|=||=|    %zZSK%z   |=||=|    %zZSK%z   |=|\n", set_output_color(lblue,white), set_output_color(black, white),set_output_color(lblue,white), set_output_color(black, white));
    xprintf(" /====|_____|====O |     |__________|    |__________|          \n");
    xprintf("  {___}      L____/        O      O        O      O            \n");
}

void zsk_move(uint32_t delay)
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

    msleep(delay);

}

void zsk(char* delay_str)
{
    uint32_t delay;

    if(!strlen(delay_str))
        delay = 100;
    
    else
        delay = strtoi(delay_str,10);

    zsk_init();

    while(getscan() != ENTER)
        zsk_move(delay);

    app_exited = true;
}