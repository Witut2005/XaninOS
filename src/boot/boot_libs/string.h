
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

uint32_t strlen(const char* a);
bool bstrcmp(const char* a, const char* b);
char* strrev(char* str);
char* int_to_string(uint32_t value, char* buf, const uint8_t base);