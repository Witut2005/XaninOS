#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void canvas_xscanf(char* str, ...);
void canvas_xscan_range(char* string_buffer, uint32_t how_many_chars);

#ifdef __cplusplus
}
#endif
