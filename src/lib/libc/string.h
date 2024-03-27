

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/macros.h>
#include <fs/xin_structures.h>
#include <lib/libc/file.h>

#define BAD_IP_ADDRESS 0xFFFFFFFF

#define STRING_UNSIGNED 0
#define STRING_SIGNED 1

enum STRTOI_OPTIONS
{
    BINARY = 2,
    OCTAL = 8,
    DECIMAL = 10,
    HEXADECIMAL = 16
};

enum STRING_ERNNO_VALUES
{
    IPV4_ERRNO = 0x1
};

#define STRCAT_DEST_FIRST true
#define STRCAT_SRC_FIRST false

#ifdef __cplusplus
extern "C"
{
#endif

    [[nodiscard]] bool is_digit(char c);
    [[nodiscard]] bool is_in_char_range(char r1, char r2, char c);
    [[nodiscard]] bool is_char(char c);
    [[nodiscard]] int char_find(const char* str, char c);
    [[nodiscard]] uint32_t strlen(const char* a);
    char* strcpy(char* dest, const char* src);
    char* strncpy(char* x, const char* y, size_t size);
    char* strrev(char* str);
    [[nodiscard]] int32_t strcmp(char* a, const char* b);
    [[nodiscard]] bool bstrcmp(char* a, const char* b);
    [[nodiscard]] bool bstrncmp(char* a, const char* b, size_t string_size);

    char* int_to_decimal_string(bool _signed, int32_t value, char* buf);
    char* int_to_string(uint32_t value, char* buf, const uint8_t base);
    char* bcd_to_string(uint8_t value, char* buf);

    void erase_spaces(char* str);

    char* toupper(char* str);
    char* tolower(char* str);

    [[nodiscard]] uint32_t atoi(char* str);
    [[nodiscard]] uint32_t strtoi(const char* str, uint8_t format);
    [[nodiscard]] uint32_t str2ipv4(char* str);

    [[nodiscard]] char* substr_find(char* str, const char* substr);
    [[nodiscard]] char* substr_last_find(char* str, const char* substr);

    [[nodiscard]] char* strdup(char* str); //allocates memory
    char* strcat(bool dest_first, char* dest, char* src);

    uint32_t check_string_errors(uint32_t mask);
    [[nodiscard]] uint32_t number_of_lines_get(const char* str);
    char* string_align_end(char* const str, char filler, uint32_t count);   // puts NULL at end of string
    char* string_align_begin(char* const str, char filler, uint32_t count); // puts NULL at end of string

    [[nodiscard]] uint32_t number_of_lines_get(const char* str);
    [[nodiscard]] uint32_t size_of_biggest_line_get(const char* str);

    [[nodiscard]] char* getline(XinEntry* File, int line_id);

    char* sprintf(char* str, char* fmt, ...);

#ifdef __cplusplus
}
#endif