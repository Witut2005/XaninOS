
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/memory.hpp>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <sys/devices/com/com.h>

void* operator new(size_t size)
{
    return calloc(size);
}

void* operator new(size_t size, void* ptr) //placment new
{
    return ptr;
}

void* operator new[](size_t size)
{
    return calloc(size);
}

void* operator new[](size_t size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    free(ptr);
}

void operator delete(void* ptr, size_t size)
{
    free(ptr);
}

void operator delete[](void* ptr)
{
    free(ptr);
}

void operator delete[](void* ptr, size_t size)
{
    free(ptr);
}