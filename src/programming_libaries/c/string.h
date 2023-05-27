
#pragma once

#include <stdint.h>
#include <stddef.h>

extern uint32_t strlen(const char* a);
extern char* strcpy(char* x, const char* y);
extern char* strncpy(char* x, const char* y, size_t size);
extern char* reverse_string(char* str);
extern char* float_to_string(float number, char* str);
extern uint32_t bstrcmp(char* a, const char* b);
extern uint32_t bstrncmp(char* a, const char* b, size_t string_size);
extern char* int_to_str(int x, char* buf);
extern char* bin_to_str(int x, char* buf);
extern void erase_spaces(char* buf);
extern char* toupper(char* str);
extern char* tolower(char* str);
extern char* int_to_hex_str(uint32_t x, char* buf);
extern uint32_t hex_str_to_int(char* str);
extern char* xint_to_hex_str(uint32_t x, char* buf, uint8_t how_many_chars);
extern char* int_to_oct_str(int x, char* buf);
extern uint32_t atoi(char* str);
extern uint32_t strtoi(char* str, uint8_t format);