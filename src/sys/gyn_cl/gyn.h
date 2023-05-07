

#pragma once

#include <stddef.h>
#include <fs/xin.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/memory.h>

extern void scan(void);

int gyn_interpreter(char* file_to_interpret);
