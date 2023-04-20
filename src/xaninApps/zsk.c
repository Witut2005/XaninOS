#pragma once

#include <libc/stdiox.h>
#include <libc/stdlibx.h>
#include <libc/time.h>
#include <libc/string.h>



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
    xprintf("  |_________________|=||=|    %zZSK%z   |=||=|    %zZSK%z   |=|\n", OUTPUT_COLOR_SET(lblue,white), OUTPUT_COLOR_SET(black, white),OUTPUT_COLOR_SET(lblue,white), OUTPUT_COLOR_SET(black, white));
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

int zsk(char* delay_str)
{
    uint32_t delay;


    if(!strlen(delay_str))
        delay = 100;
    
    else
        delay = strtoi(delay_str, DECIMAL);

    xprintf("delay: %d", delay);
    //fwhile(1);

    zsk_init();

    while(KeyInfo.scan_code != ENTER)
        zsk_move(delay);

    app_exited = true;
    return XANIN_OK;
}