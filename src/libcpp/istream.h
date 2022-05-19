
#pragma once

#include <stdint.h>
#include <libcpp/cstdio.h>
#include <libcpp/string.h>

namespace std
{


    class istream
    {

        private:
        uint8_t current_format_option{'d'};

        public:
        istream& operator >> (ostream_options x)
        {
           
            if(x.option == '\n')
                xscanf("\n");
            else if(x.option == 'h')
                current_format_option = 'h';
            else if(x.option == 'd')
                current_format_option = 'd';
            else if(x.option == 'o')
                current_format_option = 'o';
            else if(x.option == 'b')
                current_format_option = 'b';
            else 
                current_format_option = 'd';
        }

        istream& operator>>(int& x)
        {
            switch(current_format_option)
            {

                case 'h':
                    xscanf("%x", &x);
                case 'o':
                    xscanf("%o", &x);
                case 'b':
                    xscanf("%b", &x);
                case 'd':
                    xscanf("%d", &x);
            }
        }

        istream& operator>>(char& x)
        {
            xscanf("%c",&x);
        }

        istream& operator>>(short& x)
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
        }

        istream& operator>>(const char* x)
        {
            xscanf("%s", x);
        }

        istream& operator>>(char* x)
        {
            xscanf("%s", x);
        }   

        istream& operator>>(std::string& x)
        {
            x.reserve(80);
            xscanf("%s", x.c_str());
        }   
        
        istream& operator>>(std::string* x)
        {
            x->reserve(80);
            xscanf("%s", x->c_str());
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
        }


    }cin;


}
