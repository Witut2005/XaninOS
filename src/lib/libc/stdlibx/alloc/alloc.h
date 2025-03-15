#pragma once

#include "../file/file.h"
#include <lib/system/system.h>
#include <stdint.h>

static inline void* malloc(uint32_t size) { return (void*)xanin_syscall1(XANIN_ALLOCATE, (uint32_t)size); }
static inline void* calloc(uint32_t size) { return (void*)xanin_syscall1(XANIN_CALLOCATE, (uint32_t)size); }
static inline void free(void* ptr) { xanin_syscall1(XANIN_FREE, (uint32_t)ptr); }
static inline void* realloc(void* ptr, uint32_t size) { return (void*)xanin_syscall2(XANIN_REALLOCATE, (uint32_t)ptr, (uint32_t)size); }
static inline uint32_t size_to_mmngr_blocks(uint32_t size) { return int_to_sectors(size); }
