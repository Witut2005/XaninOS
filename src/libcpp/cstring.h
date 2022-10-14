
#pragma once
#include <stdint.h>
#include <stddef.h>

extern "C"
{

    uint32_t strlen(const char* a);
    char* reverse_string(char* str);
    char* float_to_string(float number, char* str);
    uint32_t strcmp(char* a,char* b);
    uint32_t strncmp(char* a, char* b, size_t string_size);
    char* int_to_str(int x, char* buf);
    char* bin_to_str(int x, char* buf);
    void erase_spaces(char* buf);
    char* toupper(char* str);
    char* tolower(char* str);
    char* int_to_hex_str(uint32_t x, char* buf);
    uint32_t hex_str_to_int(char* str);
    char* xint_to_hex_str(uint32_t x, char* buf, uint8_t how_many_chars);
    char* int_to_oct_str(int x, char* buf);
    uint32_t atoi(char* str);
    uint32_t strtoi(char* str, uint8_t format);
    char* strcpy(char* x, const char* y);


}
