

#include <programs/tests/test_case.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/container/vector.hpp>

using namespace std;
using namespace std::literals;

extern "C" __STATUS __cpp_vector_test(void)
{

    for (int i = 0; i < 1000;i++) {

        TEST_CASE(oj ja tak łatwo się nie poddam)
        {
            std::vector<std::string> v;
            for (int i = 0; i < 2; i++) {
                v.push_back("a");
                v.push_back("..");
            }

            EXPECT_EQUAL(v[0], "a");
            EXPECT_EQUAL(v[1], "..");
        }

        TEST_CASE(oj ja tak łatwo się nie poddam)
        {
            std::vector<std::string> v = { "a", ".." };

            EXPECT_EQUAL(v[0], "a");
            EXPECT_EQUAL(v[1], "..");
        }

        TEST_CASE(vector constructors)
        {
            SUB_TEST_CASE(void)
            {
                vector<int> v;
                v.push_back(1);
                v.push_back(10);
                EXPECT_EQUAL(v[0], 1);
                EXPECT_EQUAL(v[-1], 10);
            }

            SUB_TEST_CASE(initliazier list)
            {
                vector<int> v = { 0, 1, 2 };
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQUAL(v[i], i);
                }
            }

            SUB_TEST_CASE(forward iterators)
            {
                string str = "abc";
                vector<char> v(str.begin(), str.end());

                for (int i = 0; i < 3; i++) {
                    EXPECT_EQUAL_FMT("%d", v[i], 'a' + i);
                }
            }

            SUB_TEST_CASE(reversed iterators)
            {
                string str = "abc";
                vector<char> v(str.rbegin(), str.rend());

                for (int i = 0; i < 3; i++) {
                    EXPECT_EQUAL_FMT("%d", v[i], 'c' - i);
                }
            }
        }

        TEST_CASE(vector modification operations)
        {
            vector<int> v = { 1,2,3 };
            v.pop_back();
            EXPECT_EQUAL(*v.rbegin(), 2);
        }

        TEST_CASE(vector iterators)
        {
            SUB_TEST_CASE(iterator)
            {
                vector<int> v;
                v.push_back(1);
                v.push_back(10);

                auto it1 = v.begin();
                auto it2 = v.begin() + 1;

                EXPECT_BELOW_FMT("0x%x", it1.data(), it2.data());
                EXPECT_TRUE(v.end().is_end());
                EXPECT_FALSE(v.begin().is_end());

                auto it3(it1);

                it1.data();
                it1.is_end();
                it1.is_rend();
                it1.is_valid();

                it1++;
                ++it1;
                it1--;
                --it1;

                EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
            }

            SUB_TEST_CASE(const iterator)
            {
                vector<int> v;
                v.push_back(1);
                v.push_back(10);

                auto it1 = v.cbegin();
                auto it2 = v.cbegin() + 1;

                EXPECT_BELOW_FMT("0x%x", it1.data(), it2.data());
                EXPECT_TRUE(v.end().is_end());
                EXPECT_FALSE(v.begin().is_end());

                auto it3(it1);

                it1.data();
                it1.is_end();
                it1.is_rend();
                it1.is_valid();

                it1++;
                ++it1;
                it1--;
                --it1;

                EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
            }

            SUB_TEST_CASE(reversed iterator)
            {
                vector<int> v;
                v.push_back(1);
                v.push_back(10);

                auto it1 = v.rbegin();
                auto it2 = v.rbegin() + 1;

                EXPECT_ABOVE_FMT("0x%x", it1.data(), it2.data());
                EXPECT_FALSE(v.rend().is_end());
                EXPECT_TRUE(v.rend().is_rend());

                auto it3(it1);

                it1.data();
                it1.is_end();
                it1.is_rend();
                it1.is_valid();

                it1++;
                ++it1;
                it1--;
                --it1;

                EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
            }

            SUB_TEST_CASE(const reversed iterator)
            {
                vector<int> v;
                v.push_back(1);
                v.push_back(10);

                auto it1 = v.crbegin();
                auto it2 = v.crbegin() + 1;

                EXPECT_ABOVE_FMT("0x%x", it1.data(), it2.data());
                EXPECT_FALSE(v.crend().is_end());
                EXPECT_TRUE(v.crend().is_rend());

                auto it3(it1);

                it1.data();
                it1.is_end();
                it1.is_rend();
                it1.is_valid();

                it1++;
                ++it1;
                it1--;
                --it1;

                EXPECT_EQUAL_FMT("0x%x", it1.data(), it3.data());
            }
        }

        TEST_CASE(vector position erase)
        {
            std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9 };

            v.erase(v.begin() + 6);

            EXPECT_EQUAL(v.size(), 9);

            for (int i = 0; i < 6;i++) {
                EXPECT_EQUAL(v[i], i);
            }

            for (int i = 6; i < 9; i++) {
                EXPECT_EQUAL(v[i], i + 1);
            }

        }

        TEST_CASE(vector range erase)
        {
            std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9 };

            v.erase(v.begin() + 1, v.begin() + 6);

            EXPECT_EQUAL(v.size(), 5);
            EXPECT_EQUAL(v[0], 0);

            for (int i = 6; i < 10;i++) {
                EXPECT_EQUAL(v[i - 5], i);
            }

        }
    }

    return XANIN_OK;
}