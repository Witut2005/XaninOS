
#pragma once

struct FmtParserInfo {
    char alignment_char;
    uint32_t alignment_count;
    char fmt;
    uint32_t entry_lenght;
};

#ifndef __cplusplus
typedef struct FmtParserInfo FmtParserInfo;
#endif

#ifdef __cplusplus
extern "C" {
#endif

char* xvsnprintf(char* str, size_t n, char* fmt, va_list args);
char* xsnprintf(char* str, size_t n, char* fmt, ...);
char* xsprintf(char* str, char* fmt, ...);
int vsnprintf(char* str, size_t n, char* fmt, va_list args);
int snprintf(char* str, size_t n, char* fmt, ...);
int sprintf(char* str, char* fmt, ...);

#ifdef __cplusplus
}
#endif

///////////////////FUNCTIONS BELOW ARE USING C++ FUNCTION NAME MANGLING //////////////////

#ifdef __cplusplus
#    include <lib/libcpp/container/vector.hpp>

enum class FmtParseExpect {
    NormalChar,
    Filler,
    FillerCounter,
    Format
};

std::vector<FmtParserInfo> fmt_parse(char* fmt);
char* new_xvsnprintf(char* str, size_t n, char* fmt, va_list args);
char* new_xsprintf(char* str, char* fmt, ...);

#endif
