

#pragma once

#include <fs/xin_structures.h>
#include <lib/libc/file.h>
#include <lib/libc/time.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/macros.h>

#define BAD_IP_ADDRESS 0xFFFFFFFF

#define STRING_UNSIGNED 0
#define STRING_SIGNED 1

enum CHAR_FIND_OPTIONS {
    CHAR_FIND_DIGITS = 0xFE,
    CHAR_FIND_LETTERS = 0xFF
};

enum STRTOI_OPTIONS {
    BINARY = 2,
    OCTAL = 8,
    DECIMAL = 10,
    HEXADECIMAL = 16
};

enum STRING_ERNNO_VALUES {
    IPV4_ERRNO = 0x1
};

#define STRCAT_DEST_FIRST true
#define STRCAT_SRC_FIRST false

#ifdef __cplusplus
extern "C" {
#endif

[[nodiscard]] bool is_digit(char c);
[[nodiscard]] bool is_in_char_range(char r1, char r2, char c);
[[nodiscard]] bool is_char(char c);

[[nodiscard]] char* char_find(char* str, char c);
[[nodiscard]] char* char_find_from_end(char* str, uint32_t offset, char c);

[[nodiscard]] uint32_t strlen(char const* a);
char* strcpy(char* dest, char const* src);
char* strncpy(char* x, char const* y, size_t size);
char* strrev(char* str);
[[nodiscard]] int32_t strcmp(char const* a, char const* b);
[[nodiscard]] bool bstrcmp(char const* a, char const* b);
[[nodiscard]] bool bstrncmp(char const* a, char const* b, size_t string_size);

char* int_to_decimal_string(bool _signed, int32_t value, char* buf);
char* int_to_string(uint32_t value, char* buf, const uint8_t base);

char* bcd_to_string(uint8_t value, char* buf);
char* bcd_stream_to_string(uint8_t* value, uint32_t value_size, char* buf);
char* time_to_string(bcd_time_t time, char* buf);
char* date_to_string(bcd_date_t date, char* buf);

void erase_spaces(char* str);

char* toupper(char* str);
char* tolower(char* str);

[[nodiscard]] uint32_t atoi(char* str);
[[nodiscard]] uint32_t strtoi(char const* str, uint8_t format);
[[nodiscard]] uint32_t str2ipv4(char const* str);

[[nodiscard]] char* substr_find(char* str, char const* substr);
[[nodiscard]] char* substr_last_find(char* str, char const* substr);

[[nodiscard]] char* strdup(char const* str);          // allocates memory
char* strcat(bool dest_first, char* dest, char* src); // dest is buffer

uint32_t check_string_errors(uint32_t mask);
[[nodiscard]] uint32_t number_of_lines_get(char const* str);
char* string_align_end(char* const str, char filler, uint32_t count);   // puts NULL at end of string
char* string_align_begin(char* const str, char filler, uint32_t count); // puts NULL at end of string

[[nodiscard]] uint32_t number_of_lines_get(char const* str);
[[nodiscard]] uint32_t size_of_biggest_line_get(char const* str);

[[nodiscard]] char* getline(XinEntry* File, int line_id);

char* xvsnprintf(char* str, size_t n, char* fmt, va_list args);
char* xsnprintf(char* str, size_t n, char* fmt, ...);
char* xsprintf(char* str, char* fmt, ...);

int vsnprintf(char* str, size_t n, char* fmt, va_list args);
int snprintf(char* str, size_t n, char* fmt, ...);
int sprintf(char* str, char* fmt, ...);

#ifdef __cplusplus
}
#endif
