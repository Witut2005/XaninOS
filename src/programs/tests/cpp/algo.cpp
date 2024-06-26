
#include <lib/libcpp/string.h>
#include <lib/libcpp/algorithm.h>
#include <programs/tests/test_case.h>

using namespace std;

extern "C" __STATUS __cpp_algo_test(void)
{

    TEST_CASE(unique_copy)
    {
        SUB_TEST_CASE()
        {
            string nicho = "nicho";
            string nicho2;
            nicho2.reserve(XANIN_PMMNGR_BLOCK_SIZE);
            auto end = unique_copy(nicho.begin(), nicho.end(), nicho2.begin(), [](char a, char b) {return false;});
            *end = '\0';
            EXPECT_EQUAL(nicho2, "nicho");
        }

        SUB_TEST_CASE(path like problem)
        {
            string path = "/uga/buga//////nicho";
            string parsed_path;
            unique_copy(path.begin(), path.end(), std::back_inserter(parsed_path), [](char a, char b) {return a == '/' && b == '/';});
            EXPECT_EQUAL(parsed_path, "/uga/buga/nicho");
        }
    }

    TEST_CASE(have intersection)
    {
        EXPECT_TRUE(std::have_intersection<uint32_t>({ 1, 10 }, { 5, 6 }));
        EXPECT_TRUE(std::have_intersection<uint32_t>({ 1, 5 }, { 4, 10 }));
        EXPECT_FALSE(std::have_intersection<uint32_t>({ 1, 3 }, { 5, 6 }));
        EXPECT_FALSE(std::have_intersection<uint32_t>({ 5, 6 }, { 1, 3 }));
    }

    TEST_CASE(std::remove)
    {
        constexpr uint32_t value_to_remove = 4;
        std::vector<uint32_t> v = { 0,1,2,3,4,5,6,7,8,9 };
        std::remove(v.begin(), v.end(), value_to_remove);

        for (int i = 0; i < value_to_remove; i++) {
            EXPECT_EQUAL(v[i], i);
        }

        for (int i = value_to_remove; i < 9; i++) {
            EXPECT_EQUAL(v[i], i + 1);
        }
    }

    return XANIN_OK;
}