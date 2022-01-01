
#pragma once

#include "../lib/stdiox.h"

void help(void)
{

    sprint(black,white,"AVAILABLE COMMANDS:");
    sprint(black,white,"petris");
    //sprint(black,white,"calc");
    sprint(black,white,"pong");
    sprint(black,white,"cls");
    //sprint(black,white,"pit_test");
    //sprint(black,white,"spaceAx");
    //sprint(black,white,"xnote");


    for(int i = 0; i < 30000;i++)
    {
        for(int j = 0; j < cpu_mhz;j++)
        {
            asm("nop");
        }
    }

    exitApp = true;
    
}