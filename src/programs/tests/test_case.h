#pragma once
#include <lib/libc/stdiox.h>

#define TEST_CASE(LABEL)
#define SUB_TEST_CASE(LABEL)

#ifdef __cplusplus

#include <lib/libcpp/ostream.h>

#define EXPECT_EQUAL(lhs, rhs) \
        if ((lhs) != (rhs)) std::print("line {}: expected lhs == rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#define EXPECT_NOT_EQUAL(lhs, rhs) \
        if ((lhs) == (rhs)) std::print("line {}: expected lhs != rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#define EXPECT_ABOVE(lhs, rhs) \
        if ((lhs) <= (rhs)) std::print("line {}: expected lhs > rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#define EXPECT_BELOW(lhs, rhs) \
        if ((lhs) >= (rhs)) std::print("line {}: expected lhs < rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#define EXPECT_ABOVE_OR_EQUAL(lhs, rhs) \
        if ((lhs) < (rhs)) std::print("line {}: expected lhs >= rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#define EXPECT_BELOW_OR_EQUAL(lhs, rhs) \
        if ((lhs) > (rhs)) std::print("line {}: expected lhs <= rhs. Failed with lhs={} rhs={}\n", __LINE__, lhs, rhs);
#define EXPECT_TRUE(lhs) \
        if (!(lhs)) std::print("line {}: expected lhs == true. Failed with lhs={}\n", __LINE__, lhs);
#define EXPECT_FALSE(lhs) \
        if ((lhs)) std::print("line {}: expected lhs == false. Failed with lhs={}\n", __LINE__, lhs);

#endif

#define EXPECT_EQUAL_FMT(format, lhs, rhs) \
        if ((lhs) != (rhs)) xprintf("line %d: expected lhs == rhs. Failed with lhs=" format " rhs=" format "\n", __LINE__, lhs, rhs);
#define EXPECT_NOT_EQUAL_FMT(format, lhs, rhs) \
        if ((lhs) == (rhs)) xprintf("line %d: expected lhs != rhs. Failed with lhs=" format " rhs=" format "\n", __LINE__, lhs, rhs);
#define EXPECT_ABOVE_FMT(format, lhs, rhs) \
        if ((lhs) <= (rhs)) xprintf("line %d: expected lhs > rhs. Failed with lhs=" format " rhs=" format "\n", __LINE__, lhs, rhs);
#define EXPECT_BELOW_FMT(format, lhs, rhs) \
        if ((lhs) >= (rhs)) xprintf("line %d: expected lhs < rhs. Failed with lhs=" format " rhs=" format "\n", __LINE__, lhs, rhs);
#define EXPECT_ABOVE_OR_EQUAL_FMT(format, lhs, rhs) \
        if ((lhs) < (rhs)) xprintf("line %d: expected lhs >= rhs. Failed with lhs=" format " rhs=" format "\n", __LINE__, lhs, rhs);
#define EXPECT_BELOW_OR_EQUAL_FMT(format, lhs, rhs) \
        if ((lhs) > (rhs)) xprintf("line %d: expected lhs <= rhs. Failed with lhs=" format " rhs=" format "\n", __LINE__, lhs, rhs);
#define EXPECT_TRUE_FMT(format, lhs) \
        if (!(lhs)) xprintf("line {}: expected lhs == true. Failed with lhs=" format "\n", __LINE__, lhs);
#define EXPECT_FALSE_FMT(format, lhs) if ((lhs)) xprintf("line {}: expected lhs == false. Failed with lhs=" format "\n", __LINE__, lhs);