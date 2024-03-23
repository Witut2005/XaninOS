#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

    void memcpy(void* dest, void* src, size_t size);  // size in bytes
    bool bmemcmp(void* dest, void* src, size_t size); // size in bytes
    void memset(void* dest, uint8_t value, size_t size);
    void memmove(void* dest, void* src, size_t size);
    void memcpy_with_skip(void* dest, void* src, size_t size, uint32_t skip); //copies every skip-th byte (last byte copies will be (src + size))

#ifdef __cplusplus
}
#endif