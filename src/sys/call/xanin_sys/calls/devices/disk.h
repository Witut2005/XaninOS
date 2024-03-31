
#pragma once

#include <stdint.h>
#include <lib/libc/system.h>
#include <sys/call/xanin_sys/ids/xanin_syscalls.h>

static inline void __disk_read(uint32_t first_sector, uint32_t how_many, uint16_t* buf) { xanin_syscall3(XANIN_DISK_READ, (uint32_t)first_sector, (uint32_t)how_many, (uint32_t)buf); }
static inline void __disk_write(uint32_t first_sector, uint32_t how_many, uint16_t* buf) { xanin_syscall3(XANIN_DISK_WRITE, (uint32_t)first_sector, (uint32_t)how_many, (uint32_t)buf); }