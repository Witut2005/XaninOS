
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/memory.hpp>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <sys/devices/com/com.h>

void* operator new(size_t size) noexcept
{
    void* p = calloc(size);
    return p;
}

void* operator new(size_t size, void* ptr) noexcept
{
    return ptr;
}

void* operator new[](size_t size) noexcept
{
    void* p = calloc(size);
    return p;
}

void* operator new[](size_t size, void* ptr) noexcept
{
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    free(ptr);
}