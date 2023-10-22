
//https://en.wikipedia.org/wiki/Jenkins_hash_function

#pragma once

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

uint32_t jhash_with_length(const char* key, uint32_t length);
uint32_t jhash(const char* key);

#ifdef __cplusplus
}
#endif
