

#pragma once

#include <stddef.h>
#include <xin_fs/xin.h>
#include <libc/stdiox.h>
#include <libc/memory.h>

extern void scan(void);

void gyn_interpreter(char* file_to_interpret);

static bool gyn_cl_on;