
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <fs/xin_structures.h>
#include <lib/libc/system.h>
#include <sys/call/xanin_sys/ids/xanin_syscalls.h>

#ifdef __cplusplus
extern "C"
{
#endif
    static inline XinEntry* fopen(const char* path, char* mode) { return (XinEntry*)xanin_syscall2(XANIN_FOPEN, (uint32_t)path, (uint32_t)mode); }
    static inline size_t fread(XinEntry* entry, void* ptr, size_t counter) { return (size_t)xanin_syscall3(XANIN_FREAD, (uint32_t)entry, (uint32_t)ptr, (uint32_t)counter); }
    static inline size_t fwrite(XinEntry* entry, void* ptr, size_t counter) { return (size_t)xanin_syscall3(XANIN_FWRITE, (uint32_t)entry, (uint32_t)ptr, (uint32_t)counter); }
    static inline void fclose(XinEntry** entry) { xanin_syscall1(XANIN_FCLOSE, (uint32_t)entry); }

    static inline int open(const char* path, uint32_t mode) { return (int)xanin_syscall2(XANIN_OPEN, (uint32_t)path, (uint32_t)mode); }
    static inline size_t read(int fd, void* ptr, size_t counter) { return (size_t)xanin_syscall3(XANIN_READ, (uint32_t)fd, (uint32_t)ptr, (uint32_t)counter); }
    static inline size_t write(int fd, void* ptr, size_t counter) { return (size_t)xanin_syscall3(XANIN_WRITE, (uint32_t)fd, (uint32_t)ptr, (uint32_t)counter); }
    static inline void close(int fd) { xanin_syscall1(XANIN_CLOSE, (uint32_t)fd); }
#ifdef __cplusplus
}
#endif