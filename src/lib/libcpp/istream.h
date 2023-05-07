
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
        

        istream& operator>>(int& x)
        {
            switch(current_format_option)
            {

                case 'h':
                    xscanf("%x", &x);
                    break;
                case 'o':
                    xscanf("%o", &x);
                    break;
                case 'b':
                    xscanf("%b", &x);
                    break;
                case 'd':
                    xscanf("%d", &x);
                    break;
            }
            return (istream&)*this;
        }

        istream& operator>>(char& x)
        {
            xscanf("%c",&x);
            return *this;
        }

        istream& operator>>(short& x)
        {
            switch(current_format_option)
            {

                case 'h':
                    xscanf("%x", x);
                    break;
                case 'o':
                    xscanf("%o", x);
                    break;
                case 'b':
                    xscanf("%b", x);
                    break;
                case 'd':
                    xscanf("%d", x);
                    break;
            }
            return (istream&)*this;
        }

        istream& operator>>(const char* x)
        {
            xscanf("%s", x);
            return (istream&)*this;
        }

        istream& operator>>(char* x)
        {
            xscanf("%s", x);
            return (istream&)*this;
        }   

        istream& operator>>(std::string& x)
        {
            x.reserve(80);
            xscanf("%s", x.c_str());
            return (istream&)*this;
        }   
        
        istream& operator>>(std::string* x)
        {
            x->reserve(80);
            xscanf("%s", x->c_str());
            return (istream&)*this;
        }   

        istream& operator>>(unsigned int& x)
        {

            switch(current_format_option) 
            {

                case 'h':
                    xscanf("%x", x);
                case 'o':
                    xscanf("%o", x);
                case 'b':
                    xscanf("%b", x);
                case 'd':
                    xscanf("%d", x);
            }
            return (istream&)*this;
        }

        istream& operator>>(unsigned char& x)
        {
            switch(current_format_option)
            {

                case 'h':
                    xscanf("%x", x);
                case 'o':
                    xscanf("%o", x);
                case 'b':
                    xscanf("%b", x);
                case 'd':
                    xscanf("%d", x);
            }
            return (istream&)*this;
        }

        istream& operator>>(unsigned short& x)
        {
            switch(current_format_option)
            {

                case 'h':
                    xscanf("%x", x);
                case 'o':
                    xscanf("%o", x);
                case 'b':
                    xscanf("%b", x);
                case 'd':
                    xscanf("%d", x);
            }
            return (istream&)*this;
        }


    };
    
    inline istream cin;
    
    extern "C" 
    {
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

    extern key_info_t KeyInfo;

    }

}
