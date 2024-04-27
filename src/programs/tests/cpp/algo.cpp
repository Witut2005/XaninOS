
#include <lib/libcpp/string.h>
#include <lib/libcpp/algorithm.h>
#include <programs/tests/test_case.h>

using namespace std;

extern "C" __STATUS __cpp_algo_test(void)
{

    TEST_CASE(unique_copy)
    {
        string nicho = "nicho";
        string nicho2;
        nicho2.reserve(XANIN_PMMNGR_BLOCK_SIZE);
        auto end = unique_copy(nicho.begin(), nicho.end(), nicho2.begin(), [](char a, char b) {return false;});
        *end = '\0';
        EXPECT_EQUAL(nicho2, "nicho");
    }

    return XANIN_OK;
}