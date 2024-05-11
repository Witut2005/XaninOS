
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <lib/libc/stdlibx.h>

void* operator new(size_t size);
void* operator new(size_t size, void* ptr); // placment new

void* operator new[](size_t size);
void* operator new[](size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete(void* ptr, size_t size);

void operator delete[](void* ptr);
void operator delete[](void* ptr, size_t size);

namespace std
{

class LibBaseAllocator {
public:
    void* malloc(size_t size) { return malloc(size); }
    void* calloc(size_t size) { return calloc(size); }
    void* realloc(void* p, size_t size) { return realloc(p, size); }
    void free(void* p) { free(p); }
};

}
