
#include "../ustar/ustar.h"

void type()
{
    for(int i=0; i < fileSystem.file_entries_number; i++)
    {
        xprintf("\r%s\n",fileTable[i]);
        xprintf("%s\n",fileData[i]);
    }

    xprintf("\n");
    xprintf("\npress 'q' key to continue...");
    
    
    while(1)
    {
        if(input == 'q')
        {
            {exitApp = true;break;}
        }
    }

}