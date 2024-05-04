
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
