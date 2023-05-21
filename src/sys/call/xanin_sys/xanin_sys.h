
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void __disk_read(uint32_t first_sector, uint32_t how_many, uint16_t* buf)__attribute__((fastcall));
void __disk_write(uint32_t first_sector, uint32_t how_many, uint16_t* buf)__attribute__((fastcall));

#ifdef __cplusplus
}
#endif