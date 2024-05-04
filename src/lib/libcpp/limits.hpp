
#pragma once

#include <float.h>
#include <limits.h>
#include <stdint.h>

namespace std {

template<typename T>
struct numeric_limits {};

template<>
struct numeric_limits<bool>
{
    using value_type = bool;
    static constexpr value_type min(void) { return false; }
    static constexpr value_type max(void) { return true; }
};

template<>
struct numeric_limits<char>
{
    using value_type = char;
    static constexpr value_type min(void) { return CHAR_MIN; }
    static constexpr value_type max(void) { return CHAR_MAX; }
};

template<>
struct numeric_limits<unsigned char>
{
    using value_type = unsigned char;
    static constexpr value_type min(void) { return 0; }
    static constexpr value_type max(void) { return UCHAR_MAX; }
};

template<>
struct numeric_limits<short>
{
    using value_type = short;
    static constexpr value_type min(void) { return SHRT_MIN; }
    static constexpr value_type max(void) { return SHRT_MAX; }
};

template<>
struct numeric_limits<unsigned short>
{
    using value_type = unsigned short;
    static constexpr value_type min(void) { return SHRT_MIN; }
    static constexpr value_type max(void) { return SHRT_MAX; }
};

template<>
struct numeric_limits<int>
{
    using value_type = int;
    static constexpr value_type min(void) { return INT_MIN; }
    static constexpr value_type max(void) { return INT_MAX; }
};

template<>
struct numeric_limits<int32_t> : numeric_limits<int> {};

template<>
struct numeric_limits<unsigned int>
{
    using value_type = unsigned int;
    static constexpr value_type min(void) { return 0; }
    static constexpr value_type max(void) { return UINT_MAX; }
};

// template<>
// struct numeric_limits<uint32_t>
// {
//     using value_type = uint32_t;
//     static constexpr value_type min(void) { return 0; }
//     static constexpr value_type max(void) { return UINT_MAX; }
// };

template<>
struct numeric_limits<int64_t>
{
    using value_type = int64_t;
    static constexpr value_type min(void) { return INT64_MIN; }
    static constexpr value_type max(void) { return INT64_MAX; }
};

template<>
struct numeric_limits<uint64_t>
{
    using value_type = uint64_t;
    static constexpr value_type min(void) { return 0; }
    static constexpr value_type max(void) { return UINT64_MAX; }
};

template<>
struct numeric_limits<float>
{
    using value_type = float;
    static constexpr float min(void) { return FLT_MIN; }
    static constexpr float max(void) { return FLT_MAX; }
};

template<>
struct numeric_limits<double>
{
    using value_type = double;
    static constexpr value_type min(void) { return DBL_MIN; }
    static constexpr value_type max(void) { return DBL_MAX; }
};

template<>
struct numeric_limits<long double>
{
    using value_type = long double;
    static constexpr value_type min(void) { return LDBL_MIN; }
    static constexpr value_type max(void) { return LDBL_MAX; }
};

}