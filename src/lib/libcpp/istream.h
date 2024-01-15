
#pragma once

#include <stdint.h>
#include <lib/libc/stdiox.h>
#include <lib/libcpp/string.h>
#include <sys/input/key_info.h>

namespace std
{

    class istream
    {

    private:
        uint8_t current_format_option{'d'};

    public:
        istream &operator>>(int &x);
        istream &operator>>(char &x);
        istream &operator>>(short &x);
        istream &operator>>(const char *x);
        istream &operator>>(char *x);
        istream &operator>>(std::string &x);
        istream &operator>>(std::string *x);
        istream &operator>>(unsigned int &x);
        istream &operator>>(unsigned char &x);
        istream &operator>>(unsigned short &x);
    };
}
