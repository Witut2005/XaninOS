
#pragma once

#include <stdint.h>
#include <libcpp/string.h>
#include <libcpp/cstdio.h>

namespace std
{


struct ostream_options
{
    uint8_t option;
};

enum OstreamOptions
{
    endl = '\n',
    hex = 'h',
    dec = 'd',
    oct = 'o',
    bin = 'b',
    clear ='c'

};



class ostream 
{
    private:
    uint8_t current_format_option {'d'};

    public:

    ostream& operator<<(OstreamOptions x)
    {
       
        if(x == endl)
            xprintf("\n");
        
        else if(x == hex)
            current_format_option = 'h';
        
        else if(x == dec)
            current_format_option = 'd';

        else if(x == oct)
            current_format_option = 'o';

        else if(x == bin)
            current_format_option = 'b';
        
        else if(x == clear)
            screen_clear();
        
        else 
            current_format_option = 'd';

        return (ostream&)*this;

    }


    ostream& operator<<(uint32_t x)
    {

        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
                break;
            case 'o':
                xprintf("%o", x);
                break;
            case 'b':
                xprintf("%b", x);
                break;

            case 'd':
                xprintf("%d", x);
                break;
                
        }
        return (ostream&)*this;
    }

    ostream& operator<<(uint32_t* x)
    {

        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
                break;
            case 'o':
                xprintf("%o", x);
                break;
            case 'b':
                xprintf("%b", x);
                break;

            case 'd':
                xprintf("%d", x);
                break;
                
        }
        return (ostream&)*this;
    }

    ostream& operator<<(std::string x)
    {
        xprintf("%s", x.c_str());
        return (ostream&)*this;
    }   

    ostream& operator<<(int x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
                break;
            case 'o':
                xprintf("%o", x);
                break;
            case 'b':
                xprintf("%b", x);
                break;
            case 'd':
                xprintf("%d", x);
                break;
        }
        return (ostream&)*this;
    }

    ostream& operator<<(int* x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
                break;
            case 'o':
                xprintf("%o", x);
                break;
            case 'b':
                xprintf("%b", x);
                break;
            case 'd':
                xprintf("%d", x);
                break;
        }
        return (ostream&)*this;
    }


    ostream& operator<<(char x)
    {
        if(x == '\n')
        {
            xprintf("\n");
            return (ostream&)*this;
        }

        xprintf("%c",x);
        return (ostream&)*this;

    }

    ostream& operator<<(uint8_t x)
    {
        xprintf("%c",x);
        return (ostream&)*this;
    }

    ostream& operator<<(uint16_t x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
                break;    
            case 'o':
                xprintf("%o", x);
                break;
            case 'b':
                xprintf("%b", x);
                break;
            case 'd':
                xprintf("%d", x);
                break;
        }
        return (ostream&)*this;
    }

    ostream& operator<<(const char* x)
    {
        xprintf("%s", x);
        return (ostream&)*this;
    }

    ostream& operator<<(char* x)
    {
        xprintf("%s", x);
        return (ostream&)*this;
    }


    ostream& operator<<(int16_t x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
                break;
            case 'o':
                xprintf("%o", x);
                break;
            case 'b':
                xprintf("%b", x);
                break;
            case 'd':
                xprintf("%d", x);
                break;
        }
        return (ostream&)*this;

    }

    void put(char x)
    {
        xprintf("%c", x);
    }



}cout;


}
