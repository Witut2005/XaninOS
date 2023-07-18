
#pragma once

typedef uint8_t stdio_mode_t;

#ifdef __cplusplus
extern "C" {
#endif

void stdio_mode_set(stdio_mode_t mode) __attribute__((fastcall));
stdio_mode_t stdio_mode_get(void);

#ifdef __cplusplus
}
#endif