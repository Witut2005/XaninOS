

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

    TEST_CASE(vector range operations)
    {
        nvector<int> v;
        v.push_back(1);
        v.push_back(10);

        for (auto a : v)
        {
            print("{}\n", a);
        }
    }

    TEST_CASE(vector iterators)
    {
        nvector<int> v;
        v.push_back(1);
        v.push_back(10);

        auto cit = v.cbegin();
        auto var = *cit;

        v.cbegin() >= v.cend();
    }

    return XANIN_OK;
}