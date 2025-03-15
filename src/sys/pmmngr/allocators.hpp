
#pragma once
#include <stddef.h>
#include <stdint.h>

extern "C" {
    void* kmalloc(uint32_t size);
    void* kcalloc(uint32_t size);
    void kfree(void* ptr);
    void* krealloc(void* ptr, uint32_t size);
}

