
#pragma once

#include <lib/libc/stdlibx.h>
#include <sys/input/key_info.h>

#ifdef __cplusplus
extern "C"{
#endif

key_info_t __keyinfo_get(void);
char inputc(void);
xchar inputg(void);

#ifdef __cplusplus
}
#endif

extern key_info_t KeyInfo;