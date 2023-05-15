
#pragma once

#include <stdint.h>
#include <fs/xin_entry.h>

#ifdef __cplusplus
extern "C"{
namespace xin{
#endif

extern XinEntry* fopen(char* path, char* mode) __attribute__((fastcall));
extern uint32_t fread(XinEntry* entry, void* ptr, uint32_t counter) __attribute__((fastcall));
extern uint32_t fwrite(XinEntry* entry, void* ptr, uint32_t counter) __attribute__((fastcall));
extern void fclose(XinEntry** entry) __attribute__((fastcall));

extern XinEntry* open(char* path, uint32_t mode) __attribute__((fastcall));
extern uint32_t read(uint32_t fd, void* ptr, uint32_t counter) __attribute__((fastcall));
extern uint32_t write(uint32_t fd, void* ptr, uint32_t counter) __attribute__((fastcall));
extern void close(uint32_t fd) __attribute__((fastcall));

#ifdef __cplusplus
}
}
#endif