
#pragma once
#include <stdint.h>

namespace std
{

template<class input_type, class f_type>
f_type for_each(input_type begin, input_type end, f_type function )
{

    for(input_type i = begin; i != end; i++)
    {
       function(*i); 
    }

    return function;

}


template<class T>
T& max(T& x, T& y)
{
    return (x > y) ? x : y; 
}

template<class T>
T& min(T& x, T& y)
{
    return (x < y) ? x : y; 
}

template<class T, class X>
bool equal(T* beg, T* end, X* beg2)
{
        
    for(T* x = beg; x != end; x++, beg++)
    {
        if(*x != *beg)
            return false;
    }

    return true;

}




}
