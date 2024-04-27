#pragma once
#include <lib/libc/stdiox.h>

#define TEST_CASE(LABEL)
#define SUB_TEST_CASE(LABEL)

#ifdef __cplusplus

#    include <lib/libcpp/ostream.h>

#    define EXPECT_EQUAL(lhs, rhs) \
        if ((lhs) != (rhs)) std::print("line {}: expected lhs == rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#    define EXPECT_NOT_EQUAL(lhs, rhs) \
        if ((lhs) == (rhs)) std::print("line {}: expected lhs != rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#    define EXPECT_ABOVE(lhs, rhs) \
        if ((lhs) <= (rhs)) std::print("line {}: expected lhs > rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#    define EXPECT_BELOW(lhs, rhs) \
        if ((lhs) >= (rhs)) std::print("line {}: expected lhs < rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#    define EXPECT_ABOVE_OR_EQUAL(lhs, rhs) \
        if ((lhs) < (rhs)) std::print("line {}: expected lhs >= rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#    define EXPECT_BELOW_OR_EQUAL(lhs, rhs) \
        if ((lhs) > (rhs)) std::print("line {}: expected lhs <= rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);

#endif

#define EXPECT_EQUAL_FMT(format, lhs, rhs) \
    if ((lhs) != (rhs)) xprintf("line %d: expected lhs == rhs. Failed with lhs=" format " rhs=" format, __LINE__, lhs, rhs);
#define EXPECT_NOT_EQUAL_FMT(format, lhs, rhs) \
    if ((lhs) == (rhs)) xprintf("line %d: expected lhs != rhs. Failed with lhs=" format " rhs=" format, __LINE__, lhs, rhs);
#define EXPECT_ABOVE_FMT(format, lhs, rhs) \
    if ((lhs) <= (rhs)) xprintf("line %d: expected lhs > rhs. Failed with lhs=" format " rhs=" format, __LINE__, lhs, rhs);
#define EXPECT_BELOW_FMT(format, lhs, rhs) \
    if ((lhs) >= (rhs)) xprintf("line %d: expected lhs < rhs. Failed with lhs=" format " rhs=" format, __LINE__, lhs, rhs);
#define EXPECT_ABOVE_OR_EQUAL_FMT(format, lhs, rhs) \
    if ((lhs) < (rhs)) xprintf("line %d: expected lhs >= rhs. Failed with lhs=" format " rhs=" format, __LINE__, lhs, rhs);
#define EXPECT_BELOW_OR_EQUAL_FMT(format, lhs, rhs) \
    if ((lhs) > (rhs)) xprintf("line %d: expected lhs <= rhs. Failed with lhs=" format " rhs=" format, __LINE__, lhs, rhs);
