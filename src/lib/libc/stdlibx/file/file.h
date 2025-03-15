#include <fs/xin.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

uint32_t int_to_sectors(uint32_t num);
char* getline(XinEntry* File, int line_id);

#ifdef __cplusplus
}
#endif
