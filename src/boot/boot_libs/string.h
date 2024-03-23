
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

uint32_t strlen(const char* a);
bool bstrcmp(char* a, const char* b);
char* strrev(char* str);
char* int_to_hex_str(uint32_t x, char* buf);