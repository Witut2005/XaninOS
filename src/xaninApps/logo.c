
#include <terminal/vty.h>

void logo_color_change(char* color)
{

    if(cmpstr(color, "black"))
        logo_front_color = black;


    else if(cmpstr(color, "blue"))
        logo_front_color = blue; 

    else if(cmpstr(color, "green"))
        logo_front_color = green;

    else if(cmpstr(color, "cyan"))
        logo_front_color = cyan; 

    else if(cmpstr(color, "red"))
        logo_front_color = red;
    
    else if(cmpstr(color, "magenta"))
        logo_front_color = magenta;
    
    else if(cmpstr(color, "brown"))
        logo_front_color = brown;
    
    else if(cmpstr(color, "lgray"))
        logo_front_color = lgray;
    
    else if(cmpstr(color, "dgray"))
        logo_front_color = dgray;

    else if(cmpstr(color, "lblue"))
        logo_front_color = lblue;
    
    else if(cmpstr(color, "lgreen"))
        logo_front_color = lgreen;
    
    else if(cmpstr(color, "lcyan"))
        logo_front_color = lcyan;
    
    else if(cmpstr(color, "lred"))
        logo_front_color = lred;
    
    else if(cmpstr(color, "lmagenta"))
        logo_front_color = lmagenta;

    else if(cmpstr(color, "yellow"))
        logo_front_color = yellow;

    else if(cmpstr(color, "white"))
        logo_front_color = white;

    //uint16_t tmp = strtoi(color,16);

    //logo_front_color = tmp & 0xF;
    //logo_back_color = tmp >> 4;

}