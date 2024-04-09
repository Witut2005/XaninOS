
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

void* operator new[](size_t size) noexcept
{
    // dbg_info(DEBUG_LABEL_LIBCPP, xsprintf(std::UniquePtr<char>((char*)calloc(50)).get(), "size: %d", size));
    void* p = calloc(size);
    return p;
}

void operator delete(void* ptr) noexcept
{
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    free(ptr);
}