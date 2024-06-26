

#pragma once

#include <stdint.h>
#include <sys/call/xanin_sys/ids/xanin_syscalls.h>

#ifdef __cplusplus
extern "C" {
#endif

char* system_variable_get(char* value);
uint32_t xanin_syscall0(uint32_t syscall_id);
uint32_t xanin_syscall1(uint32_t syscall_id, uint32_t arg1);
uint32_t xanin_syscall2(uint32_t syscall_id, uint32_t arg1, uint32_t arg2);
uint32_t xanin_syscall3(uint32_t syscall_id, uint32_t arg1, uint32_t arg2, uint32_t arg3);
uint32_t xanin_syscall4(uint32_t syscall_id, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

#ifdef __cplusplus
}
#endif
