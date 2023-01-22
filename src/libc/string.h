

#pragma once

// #include <libc/math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define nullptr 0x0
#define VGA_TEXT_MEMORY 0xb8000

#ifdef __cplusplus
extern "C" {
#endif

uint32_t strlen(const char* a);
char* strcpy(char* dest, const char* src);
char* strncpy(char* x, const char* y, size_t size);
char* reverse_string(char* str);
char* float_to_string(float number, char* str);
uint32_t strcmp(char* a, const char* b);
uint32_t strncmp(char* a, const char* b, size_t string_size);
char* int_to_str(int x, char* buf);
char* bin_to_str(int x, char* buf);
char* bcd_to_str(uint8_t x, char* buf);
void erase_spaces(char* buf);
char* toupper(char* str);
char* tolower(char* str);
char* int_to_hex_str(uint32_t x, char* buf);
uint32_t hex_str_to_int(char* str);
char* xint_to_hex_str(uint32_t x, char* buf, uint8_t how_many_chars);
char* int_to_oct_str(int x, char* buf);
uint32_t atoi(char* str);
uint32_t strtoi(char* str, uint8_t format);
uint32_t str2ipv4(char* str);
char* substr_find(char *str, const char *substr);
char* strconcat(char* str1, char* buf); //concatenate str1 and buf (first str1)

#ifdef __cplusplus
}
#endif