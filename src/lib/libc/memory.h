#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <lib/libc/math.h>


#ifdef __cplusplus
extern "C" {
#endif

    void memcpy(void* dest, const void* src, size_t size);  // size in bytes
    [[nodiscard]] int32_t memcmp(const uint8_t* dest, uint8_t* src, size_t size);
    [[nodiscard]] bool bmemcmp(const void* dest, const void* src, size_t size); // size in bytes
    void memset(void* dest, uint8_t value, size_t size);
    void memmove(void* dest, const void* src, size_t size);
    void memcpy_with_skip(void* dest, void* src, size_t size, uint32_t skip); //copies every skip-th byte (last byte copies will be (src + size))

    static inline uint32_t pointers_offset_get(void* ptr1, void* ptr2) 
    {
        return abs((int)(ptr1) - (int)(ptr2));
    }

#ifdef __cplusplus
}
#endif