
#include "./istream.h"

namespace std
{

istream& istream::operator>>(int& x)
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

istream& istream::operator>>(char& x)
{
    xscanf("%c",&x);
    return *this;
}

istream& istream::operator>>(short& x)
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

istream& istream::operator>>(const char* x)
{
    xscanf("%s", x);
    return (istream&)*this;
}

istream& istream::operator>>(char* x)
{
    xscanf("%s", x);
    return (istream&)*this;
}   

istream& istream::operator>>(std::string& x)
{
    x.reserve(80);
    xscanf("%s", x.c_str());
    return (istream&)*this;
}   

istream& istream::operator>>(std::string* x)
{
    x->reserve(80);
    xscanf("%s", x->c_str());
    return (istream&)*this;
}   

istream& istream::operator>>(unsigned int& x)
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

istream& istream::operator>>(unsigned char& x)
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

istream& istream::operator>>(unsigned short& x)
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

istream cin;

}