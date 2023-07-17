#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t pow(uint32_t a, uint32_t b);
float fpow(float base, float exponent);
int abs(int num);
float fabs(float num);

#ifdef __cplusplus
}
#endif