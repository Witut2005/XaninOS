
#pragma once

#include <lib/libcpp/iterator.hpp>

namespace std { 

// base container
template<typename T>
class Container
{
    public:

    virtual T* pointer_get(void) = 0;

    virtual T& front(void) = 0;
    virtual T& back(void) = 0;
    virtual T& operator [](int32_t index) = 0; 

    virtual int size() = 0;
};

}