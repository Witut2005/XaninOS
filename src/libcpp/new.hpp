
#include <stdint.h>
#include <stddef.h>
#include <libcpp/initializer_list.hpp>
#include <libcpp/cstdio.h>



inline void* operator new[](size_t size) noexcept
{
    void* p = malloc(size);
    xprintf("size allocated: %d\n", size);
    return p;
}

inline void* operator new(size_t size) noexcept
{
    void* p = malloc(size);
    xprintf("size allocated: %d\n", size);
    return p;
}

/*
template <class T>
void* operator new(std::initializer_list<T> element)
{
    uint32_t size = element.size();

    void* p = malloc(size);
    return p;
}

*/