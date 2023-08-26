
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

enum STDIO_MODES{
    STDIO_MODE_CANVAS = 0,
    STDIO_MODE_TERMINAL = 1
};

enum XANIN_STANDARD_STREAMS{
    XANIN_STDOUT = 0,
    XANIN_STDIN = 1,
    XANIN_STDERR = 2
};
