
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

uint32_t strlen(const char* a);
bool bstrcmp(char* a, const char* b);
char* reverse_string(char* str);
char* int_to_str(int x, char* buf);
char* int_to_hex_str(uint32_t x, char* buf);