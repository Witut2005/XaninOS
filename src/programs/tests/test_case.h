#pragma once
#include <lib/libc/stdiox.h>

#define TEST_CASE(LABEL)

#ifdef __cplusplus

#include <lib/libcpp/ostream.h>

#define __EXPECT_EQUAL(lhs, rhs, line) { \
if (lhs != rhs) \
print("line {}: expected lhs == rhs. Failed with lhs={} rhs={}", line, lhs, rhs); \
}

#define EXPECT_EQUAL(lhs, rhs) __EXPECT_EQUAL(lhs, rhs, __LINE__)

#endif

#define __EXPECT_EQUAL_FMT(format, lhs, rhs, line) { \
    if (lhs != rhs) \
        xprintf("line %d: expected lhs == rhs. Failed with lhs=" format "rhs=" format, __LINE__, lhs, rhs); \
}

#define EXPECT_EQUAL_FMT(format, lhs, rhs) __EXPECT_EQUAL_FMT(format, lhs, rhs, __LINE__)