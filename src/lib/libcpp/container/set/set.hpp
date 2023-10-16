
#pragma once

#include <lib/libcpp/list.h>
#include <lib/libcpp/initializer_list.hpp>

template <class T>
class set
{
    private:
    std::List<T> ListItem;
    
    public:

    set(std::initializer_list<T> items) : ListItem(items)
    {
        //ListItem. 
    }

};
