
#include <stdint.h>
#include <stddef.h>

void* operator new(size_t size) noexcept;
void* operator new[](size_t size) noexcept;

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
/*
template <class T>
void* operator new(std::initializer_list<T> element)
{
    uint32_t size = element.size();

    void* p = malloc(size);
    return p;
}

*/