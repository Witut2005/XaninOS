

#include <lib/libcpp/container/vector.hpp>
#include <programs/tests/test_case.h>

using namespace std;

extern "C" __STATUS __cpp_vector_test(void)
{

    TEST_CASE(vector constructors)
    {
        nvector<int> v;
        v.push_back(1);
        v.push_back(10);
        EXPECT_EQUAL(v[0], 1);
        EXPECT_EQUAL(v[-1], 10);
    }

    TEST_CASE(vector iterators)
    {
        SUB_TEST_CASE(iterator)
        {
            nvector<int> v;
            v.push_back(1);
            v.push_back(10);

            auto it1 = v.begin();
            auto it2 = v.begin() + 1;

            EXPECT_BELOW_FMT("0x%x", it1.data(), it2.data());
            EXPECT_TRUE(v.end().is_end());
            EXPECT_FALSE(v.begin().is_end());

            auto it3(it1);

            it1++;
            ++it1;
            it1--;
            --it1;

            EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
        }

        SUB_TEST_CASE(const iterator)
        {
            nvector<int> v;
            v.push_back(1);
            v.push_back(10);

            auto it1 = v.cbegin();
            auto it2 = v.cbegin() + 1;

            EXPECT_BELOW_FMT("0x%x", it1.data(), it2.data());
            EXPECT_TRUE(v.end().is_end());
            EXPECT_FALSE(v.begin().is_end());

            auto it3(it1);

            it1++;
            ++it1;
            it1--;
            --it1;

            EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
        }

        SUB_TEST_CASE(reversed iterator)
        {
            nvector<int> v;
            v.push_back(1);
            v.push_back(10);

            auto it1 = v.rbegin();
            auto it2 = v.rbegin() + 1;

            EXPECT_ABOVE_FMT("0x%x", it1.data(), it2.data());
            EXPECT_FALSE(v.rend().is_end());
            EXPECT_TRUE(v.rend().is_rend());

            auto it3(it1);

            it1++;
            ++it1;
            it1--;
            --it1;

            EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
        }

        SUB_TEST_CASE(const reversed iterator)
        {
            nvector<int> v;
            v.push_back(1);
            v.push_back(10);

            auto it1 = v.crbegin();
            auto it2 = v.crbegin() + 1;

            EXPECT_ABOVE_FMT("0x%x", it1.data(), it2.data());
            EXPECT_FALSE(v.crend().is_end());
            EXPECT_TRUE(v.crend().is_rend());

            auto it3(it1);

            it1++;
            ++it1;
            it1--;
            --it1;

            EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
        }
    }

    return XANIN_OK;
}