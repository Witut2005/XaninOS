
#include "../terminal/vty.h"
#include "../ustar/ustar.h"
#include "../lib/stdiox.h"
#include "../handlers/handlers.h"
#include "../lib/signal.h"



void ls()
{


    for(int i=0; i < sizeof(fileTable) / sizeof(fileTable[0]); i++)
        sprint(red,white, fileTable[i]);

    sprint(black,white,"press 'q' key to continue...");
    
    
    while(1)
    {
        if(input == 'q')
        {
            {exitApp = true;break;}
        }
    }


}