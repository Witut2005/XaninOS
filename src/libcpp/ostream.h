
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
    }

    ostream& operator<<(unsigned int x)
    {

        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
            case 'o':
                xprintf("%o", x);
            case 'b':
                xprintf("%b", x);
            case 'd':
                xprintf("%d", x);
        }
    }

    ostream& operator<<(std::string x)
    {
        xprintf("%s", x.c_str());
    }   

    ostream& operator<<(int x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
            case 'o':
                xprintf("%o", x);
            case 'b':
                xprintf("%b", x);
            case 'd':
                xprintf("%d", x);


        }
    }


    ostream& operator<<(char x)
    {
        if(x == '\n')
        {
            xprintf("\n");
            return *this;
        }

        xprintf("%c",x);
    }

    ostream& operator<<(unsigned char x)
    {
        xprintf("%c",x);
    }

    ostream& operator<<(unsigned short x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
            case 'o':
                xprintf("%o", x);
            case 'b':
                xprintf("%b", x);
            case 'd':
                xprintf("%d", x);


        }
    }

    ostream& operator<<(const char* x)
    {
        xprintf("%s", x);
    }

    ostream& operator<<(char* x)
    {
        xprintf("%s", x);
    }

    ostream& operator<<(short x)
    {
        switch(current_format_option)
        {

            case 'h':
                xprintf("%x", x);
            case 'o':
                xprintf("%o", x);
            case 'b':
                xprintf("%b", x);
            case 'd':
                xprintf("%d", x);


        }
    }

    void put(char x)
    {
        xprintf("%c", x);
    }



}cout;


}
