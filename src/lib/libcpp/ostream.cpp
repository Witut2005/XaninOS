
#include <lib/libc/stdiox.h>
#include "./ostream.h"

namespace std 
{

ostream& ostream::operator<<(OstreamOptions x)
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

ostream& ostream::operator<<(uint32_t x)
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

ostream& ostream::operator<<(int x)
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

ostream& ostream::ostream::operator<<(int* x)
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


    ostream& ostream::operator<<(char x)
    {
        if(x == '\n')
        {
            xprintf("\n");
            return (ostream&)*this;
        }

        xprintf("%c",x);
        return (ostream&)*this;

    }

    ostream& ostream::operator<<(uint8_t x)
    {
        xprintf("%c",x);
        return (ostream&)*this;
    }

    ostream& ostream::operator<<(uint16_t x)
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

    ostream& ostream::operator<<(const char* x)
    {
        xprintf("%s", x);
        return (ostream&)*this;
    }

    ostream& ostream::operator<<(const std::string& x)
    {
        xprintf("%s", x.c_str());
        return (ostream&)*this;
    }

    ostream& ostream::operator<<(char* x)
    {
        xprintf("%s", x);
        return (ostream&)*this;
    }


    ostream& ostream::operator<<(int16_t x)
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

    ostream& ostream::operator<< (const CmosTime& time)
    {
        xprintf("%i:%i:%i", time.hour, time.minutes, time.seconds);
        return *this;
    }

    ostream& ostream::operator<< (const bcd& year)
    {
        xprintf("%x", year.bcd_num);
        return *this;
    }

    void ostream::put(char x)
    {
        xprintf("%c", x);
    }

    ostream cout;

}