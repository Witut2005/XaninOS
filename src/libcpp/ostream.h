
#pragma once

#include <stdint.h>
#include <libc/time.h>
#include <libcpp/string.h>
#include <libc/stdiox.h>

namespace std
{


struct ostream_options
{
    uint8_t option;
};

enum OstreamOptions
{
    endl = '\n',
    end_of_text = '\0',
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
       
        if (x == endl)
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
        
        else if(x == end_of_text)
        {
            current_format_option = 'd';
        }
        
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

    ostream& operator<<(const std::string& x)
    {
        xprintf("%s", x.c_str());
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

    ostream& operator<< (CmosTime time)
    {
        xprintf("%i:%i:%i", time.hour, time.minutes, time.seconds);
        return *this;
    }

    ostream& operator<< (bcd year)
    {
        xprintf("%x", year.bcd_num);
        return *this;
    }

    void put(char x)
    {
        xprintf("%c", x);
    }



};

inline ostream cout;


template <class X>
void print(X&& arg)
{
    std::cout << arg;
}

template <class X, class ... T>
void print(const char* str, X&& arg, T&& ... args)
{

    int i = strlen(str);

    if(!i)
        return;

    if(strncmp((char*)str, "{}", 2))
    {
        print(arg);
        print(str+2, (args)...);
    }

    else if(*str == '%')
    {
        if(strncmp((char*)str, "%d", 2))
        {
            xprintf("%d", arg);
        }

        else if(strncmp((char*)str, "%x", 2))
        {
            xprintf("%x", arg);
        }

        else if(strncmp((char*)str, "%b", 2))
        {
            xprintf("%b", arg);
        }

        print(str + 2, (args)...);

    }

    else
    {
        xprintf("%c", *str);
        print(str + 1, arg, (args)...);
    }



}

}
