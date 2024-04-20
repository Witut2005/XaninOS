
#pragma once

#include <stdint.h>
#include <lib/libc/time.h>
#include <lib/libcpp/string.h>

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
    clear = 'c'
};

typedef uint8_t* ostream_address_t;

class ostream
{
private:
    uint8_t current_format_option{ 'd' };

public:

    ostream& operator<<(OstreamOptions x);
    ostream& operator<<(char x);
    ostream& operator<<(uint8_t x);
    ostream& operator<<(uint16_t x);
    ostream& operator<<(int16_t x);
    ostream& operator<<(int x);
    ostream& operator<<(uint32_t x);
    ostream& operator<<(ostream_address_t x);
    ostream& operator<<(int* x);
    ostream& operator<<(const char* x);
    ostream& operator<<(const string& x);
    ostream& operator<<(char* x);
    ostream& operator<< (const CmosTime& time);
    ostream& operator<< (const bcd& year);
    void put(char x);

};

extern ostream cout;

template <class X>
void print(X&& arg)
{
    std::cout << arg;
}

template <class X, class ... T>
void print(const char* str, X&& arg, T&& ... args)
{

    int i = strlen(str);

    if (!i)
        return;

    if (bstrncmp((char*)str, "{}", 2))
    {
        print(arg);
        print(str + 2, (args)...);
    }

    else if (*str == '%')
    {
        if (bstrncmp((char*)str, "%d", 2))
        {
            xprintf("%d", arg);
        }

        else if (bstrncmp((char*)str, "%x", 2))
        {
            xprintf("%x", arg);
        }

        else if (bstrncmp((char*)str, "%b", 2))
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
