#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    void memcpy(uint8_t *dst, uint8_t* src, size_t size);
    bool memcmp(uint8_t *dst, uint8_t* src, size_t size);
    void memset(uint8_t *dst, uint8_t value, size_t size);
    void memmove(uint8_t* dst, uint8_t* src, size_t size);

#ifdef __cplusplus
}
#endif