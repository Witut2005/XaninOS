

#pragma once




// #include <lib/libc/math.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define BAD_IP_ADDRESS 0xFFFFFFFF

enum STRTOI_OPTIONS{
    BINARY = 2,
    OCTAL = 8,
    DECIMAL = 10,
    HEXADECIMAL = 16
};

struct StringRectangle
{
    uint32_t size_x;
    uint32_t size_y;
    uint32_t position_x;
    uint32_t position_y;
};

typedef struct StringRectangle StringRectangle;

enum STRING_ERNNO_VALUES{
    IPV4_ERRNO = 0x1
};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t strlen(const char* a);
char* strcpy(char* dest, const char* src);
char* strncpy(char* x, const char* y, size_t size);
char* reverse_string(char* str);
char* float_to_string(float number, char* str);
uint32_t strcmp(char* a, const char* b);
bool bstrcmp(char* a, const char* b);
bool bstrncmp(char* a, const char* b, size_t string_size);
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
uint32_t strtoi(const char* str, uint8_t format);
uint32_t str2ipv4(char* str);
char* substr_find(char *str, const char *substr);
char* strconcat(char* str1, char* buf); //concatenate str1 and buf (first str1)
uint32_t check_string_errors(uint32_t mask);
uint32_t number_of_lines_get(const char* str);

#ifdef __cplusplus
}
#endif