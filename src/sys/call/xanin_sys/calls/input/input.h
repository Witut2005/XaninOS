
#pragma once

#include <sys/input/key_info.h>

#ifdef __cplusplus
extern "C" {
#endif

char __sys_getchar(void);
char __sys_getscan(void);
xchar __sys_getxchar(void);
xchar __sys_inputg(void);
// key_info_t __sys_keyinfo_get(void);
void __sys_keyinfo_get(key_info_t*);

#ifdef __cplusplus
}
#endif