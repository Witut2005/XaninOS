
#pragma once
#include <stdint.h>

template<class input_type, class f_type>
f_type for_each(input_type begin, input_type end, f_type function )
{

    for(input_type i = begin; i != end; i++)
    {
       function(*i); 
    }

    return function;

}
