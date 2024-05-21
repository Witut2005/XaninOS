
#include <lib/libcpp/ostream.h>
#include <programs/tests/test_case.h>
#include <lib/libcpp/container/array/array.hpp>

using namespace std;
using namespace std::literals;

#define ARRAY_TEST_NUMBER_OF_LOOPS 1

extern "C" __STATUS __cpp_array_test(void)
{
    for(int i = 0; i < ARRAY_TEST_NUMBER_OF_LOOPS; i++) 
    {
        TEST_CASE(array constructors)
        {
            SUB_TEST_CASE(initializer list)
            {
                std::array<int, 10> arr = {0,1,2,3,4,5,6,7,8,9}; 
                for(int i = 0; i < arr.size(); i++) {
                    EXPECT_EQUAL(arr[i], i);
                }
                
                int counter = 0;
                for(auto& a : arr) {
                    EXPECT_EQUAL(a, counter++);
                }
            }

            SUB_TEST_CASE(copy constructor) 
            {
                std::array<int, 10> arr1 = {0,1,2,3,4,5,6,7,8,9}; 
                auto arr2 = arr1;

                for(int j = 0; j < arr2.size(); j++) {
                    EXPECT_EQUAL(arr2[j], j);
                }

                int counter = 0;
                for(auto& a : arr2) {
                    EXPECT_EQUAL(a, counter++);
                }

                EXPECT_JUST_TRUE(arr2 == arr2);
                EXPECT_JUST_FALSE(arr2 != arr2);
            }
        } 

        TEST_CASE(array iterators)
        {
            SUB_TEST_CASE(iterator)
            {
                std::array<int, 10> arr = {0,1,2,3,4,5,6,7,8,9}; 

                auto counter = 0;
                for(auto beg = arr.begin(); beg != arr.end(); beg++, counter++) {
                    EXPECT_EQUAL(*beg, counter);
                }
            }

            SUB_TEST_CASE(reversed iterator)
            {
                std::array<int, 10> arr = {0,1,2,3,4,5,6,7,8,9}; 

                auto counter = 1;
                for(auto beg = arr.rbegin(); beg != arr.rend(); beg++, counter++) {
                    EXPECT_EQUAL(*beg, arr.size() - counter);
                }
            }

            SUB_TEST_CASE(const iterator)
            {
                std::array<int, 10> arr = {0,1,2,3,4,5,6,7,8,9}; 
                auto cit = arr.cbegin();
                // *cit = 10; //uncomment to check if throws an error
            }
        }
    }
    return XANIN_OK;
}