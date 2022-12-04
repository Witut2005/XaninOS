
#include <terminal/vty.h>

bool is_logo_color_blocked;

int logo_color_change(char* color, char* options)
{
    if(strcmp(options, "block"))
        is_logo_color_blocked = true;
    else if(strcmp(options, "unblock")) 
        is_logo_color_blocked = false;


    if(strcmp(color, "black"))
        logo_front_color = black;


    else if(strcmp(color, "blue"))
        logo_front_color = blue; 

    else if(strcmp(color, "green"))
        logo_front_color = green;

    else if(strcmp(color, "cyan"))
        logo_front_color = cyan; 

    else if(strcmp(color, "red"))
        logo_front_color = red;
    
    else if(strcmp(color, "magenta"))
        logo_front_color = magenta;
    
    else if(strcmp(color, "brown"))
        logo_front_color = brown;
    
    else if(strcmp(color, "lgray"))
        logo_front_color = lgray;
    
    else if(strcmp(color, "dgray"))
        logo_front_color = dgray;

    else if(strcmp(color, "lblue"))
        logo_front_color = lblue;
    
    else if(strcmp(color, "lgreen"))
        logo_front_color = lgreen;
    
    else if(strcmp(color, "lcyan"))
        logo_front_color = lcyan;
    
    else if(strcmp(color, "lred"))
        logo_front_color = lred;
    
    else if(strcmp(color, "lmagenta"))
        logo_front_color = lmagenta;

    else if(strcmp(color, "yellow"))
        logo_front_color = yellow;

    else if(strcmp(color, "white"))
        logo_front_color = white;

    else if(strcmp(color, "rand"))
    {
        logo_front_color = rand() % 16;
        if(!logo_front_color)
            logo_front_color++;
    }

    else if(strcmp(color, "\0"))
    {
        logo_front_color = rand() % 16;
        if(!logo_front_color)
            logo_front_color++;
    }

    return XANIN_OK;
        

}