#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __x86_64__
#define PHYSICAL_ADDRESS_MAX 0xFFFFFFFFFFFFFFFF
#else
#define PHYSICAL_ADDRESS_MAX 0xFFFFFFFF
#endif

extern "C"
{

void memcpy(uint8_t *dst, uint8_t* src, size_t size);
bool memcmp(uint8_t *dst, uint8_t* src, size_t size);
void memset(uint8_t *dst, uint8_t value, size_t size);

}