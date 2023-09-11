
#include <sys/screen/screen.h>

bool is_logo_color_blocked;

int logo_color_change(char* color, char* options)
{
    if(bstrcmp(options, "block"))
        is_logo_color_blocked = true;
    else if(bstrcmp(options, "unblock")) 
        is_logo_color_blocked = false;


    if(bstrcmp(color, "black"))
        logo_front_color = black;


    else if(bstrcmp(color, "blue"))
        logo_front_color = blue; 

    else if(bstrcmp(color, "green"))
        logo_front_color = green;

    else if(bstrcmp(color, "cyan"))
        logo_front_color = cyan; 

    else if(bstrcmp(color, "red"))
        logo_front_color = red;
    
    else if(bstrcmp(color, "magenta"))
        logo_front_color = magenta;
    
    else if(bstrcmp(color, "brown"))
        logo_front_color = brown;
    
    else if(bstrcmp(color, "lgray"))
        logo_front_color = lgray;
    
    else if(bstrcmp(color, "dgray"))
        logo_front_color = dgray;

    else if(bstrcmp(color, "lblue"))
        logo_front_color = lblue;
    
    else if(bstrcmp(color, "lgreen"))
        logo_front_color = lgreen;
    
    else if(bstrcmp(color, "lcyan"))
        logo_front_color = lcyan;
    
    else if(bstrcmp(color, "lred"))
        logo_front_color = lred;
    
    else if(bstrcmp(color, "lmagenta"))
        logo_front_color = lmagenta;

    else if(bstrcmp(color, "yellow"))
        logo_front_color = yellow;

    else if(bstrcmp(color, "white"))
        logo_front_color = white;

    else if(bstrcmp(color, "rand"))
    {
        logo_front_color = rand() % 16;
        if(!logo_front_color)
            logo_front_color++;
    }

    else if(bstrcmp(color, "\0"))
    {
        logo_front_color = rand() % 16;
        if(!logo_front_color)
            logo_front_color++;
    }

    return XANIN_OK;
        

}