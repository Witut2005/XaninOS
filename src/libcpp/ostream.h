
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


ostream_options endl{'\n'};
ostream_options hex {'h'};
ostream_options dec {'d'};
ostream_options oct {'o'};
ostream_options bin {'b'};
ostream_options clear {'c'};

class ostream 
{
    private:
    uint8_t current_format_option {'d'};

    public:

    ostream& operator<<(ostream_options x)
    {
       
        if(x.option == '\n')
            xprintf("\n");
        else if(x.option == 'h')
            current_format_option = 'h';
        else if(x.option == 'd')
            current_format_option = 'd';
        else if(x.option == 'o')
            current_format_option = 'o';
        else if(x.option == 'b')
            current_format_option = 'b';
        else if(x.option == 'c')
            screen_clear();
        else 
            current_format_option = 'd';

        return (ostream&)*this;

    }

    ostream& operator<<(unsigned int x)
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

    ostream& operator<<(unsigned char x)
    {
        xprintf("%c",x);
        return (ostream&)*this;
    }

    ostream& operator<<(unsigned short x)
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


    ostream& operator<<(short x)
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
