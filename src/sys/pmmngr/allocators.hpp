
#pragma once
#include <stddef.h>
#include <stdint.h>

extern "C" {
    void* kmalloc(uint32_t size);
    void* kcalloc(uint32_t size);
    void kfree(void* ptr);
    void* krealloc(void* ptr, uint32_t size);
}


class KernelBaseAllocator {
public:
    void* malloc(size_t size) { return kmalloc(size); }
    void* calloc(size_t size) { return kcalloc(size); }
    void* realloc(void* p, size_t size) { return krealloc(p, size); }
    void free(void* p) { kfree(p); }
};