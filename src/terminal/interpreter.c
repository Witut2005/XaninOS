
#include "./interpreter.h"


void scan(void)
{



    if(cmpstr(COMMAND,"petris"))
    {
        clearScr();
        tetris();
    }



    else if(cmpstr(COMMAND,"help") || cmpstr(COMMAND,"h"))
    {
        clearScr();
        y = 2;
        x = 0;

        help();   
   }


    else if(cmpstr(COMMAND,"pong"))
    {

        pongStart:

        initPong();

        while(!exitApp)
        {
            updatePong();
        }   
        
        IRQ1_OFF();
        signal.appHandler = nullptr;

        
    }


 

    else if(cmpstr(COMMAND,"cls") || cmpstr(COMMAND,"clear"))
    {
        clearScr();
        exitApp = true;
    }

    /*
    else if(cmpstr(COMMAND,"space"))
    {
        initSpace();
        while(1);
    }

    else if(cmpstr(COMMAND,"calc") || cmpstr(COMMAND,"xcalc"))
    {
        clearScr();
        calc();
        
    }

    else if(cmpstr(COMMAND,"runner"))
    {
        //runner();
    }

    */

    for(int i = 0; i < 7;i++)
        COMMAND[i] = i;

}
