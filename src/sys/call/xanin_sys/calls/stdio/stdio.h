
#pragma once

#include <lib/system/system.h>
#include <sys/call/xanin_sys/ids/xanin_syscalls.h>

typedef uint8_t stdio_mode_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline void stdio_mode_set(stdio_mode_t mode)
{
    xanin_syscall1(XANIN_STDIO_MODE_SET, mode);
}
static inline stdio_mode_t stdio_mode_get(void) { return (stdio_mode_t)xanin_syscall0(XANIN_STDIO_MODE_GET); }

#ifdef __cplusplus
}
#endif

enum STDIO_MODES {
    STDIO_MODE_CANVAS = 0,
    STDIO_MODE_TERMINAL = 1
};

enum XANIN_STANDARD_STREAMS {
    XANIN_STDOUT = 0,
    XANIN_STDIN = 1,
    XANIN_STDERR = 2
};
