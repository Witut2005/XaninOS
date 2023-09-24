
#pragma once

#include <stdint.h>
#include <lib/libc/stdiox.h>
#include <lib/libcpp/string.h>

namespace std
{

class istream
{

    private:
    uint8_t current_format_option{'d'};

    public:
    
    istream& operator>>(int& x);
    istream& operator>>(char& x);
    istream& operator>>(short& x);
    istream& operator>>(const char* x);
    istream& operator>>(char* x);
    istream& operator>>(std::string& x);
    istream& operator>>(std::string* x);
    istream& operator>>(unsigned int& x);
    istream& operator>>(unsigned char& x);
    istream& operator>>(unsigned short& x);
};

struct key_info_t
{
    uint8_t scan_code;
    char character;

    bool is_shift;
    bool is_ctrl;
    bool is_caps;
    bool is_bspc;
    bool is_alt;

    bool is_left;
    bool is_right;
    bool is_up;
    bool is_down;
    
    bool is_pressed;
    bool is_hold;

};

extern "C" key_info_t KeyInfo;

}
