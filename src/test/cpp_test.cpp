
#include <libcpp/algorithm.h>
#include <libcpp/cstring.h>
#include <libcpp/cmemory.h>
#include <libcpp/ostream.h>
#include <libcpp/cstdlib.h>
#include <libcpp/string.h>
#include <libcpp/istream.h>
#include <keyboard/scan_codes.h>
#include <libcpp/array.cpp>
#include <libcpp/initializer_list.hpp>
#include <libcpp/new.hpp>

class Test
{
    public:
    Test(int x, int y) { std::cout << std::dec << "CLASS: " << (int)x << ", " << (int)y << std::endl; }
    void print(int x, int y) { std::cout << std::dec << "CLASS: " << (int)x << ", " << (int)y << std::endl; }
};

void cpp_prog()
{


    std::cout << std::clear;

    int* p;
    
    p = (int*)malloc(sizeof(int));
    std::cout << std::hex << p <<std::endl;
    p = new int{32};

    p = reinterpret_cast<int*> (operator new (sizeof(int)));

    //Test* hm = new Test{10, 20};

    Test* hm = reinterpret_cast<Test*>(::operator new (sizeof(Test)));

    hm->print(50, 100);

    //std::cout << std::hex << (unsigned int)(p) <<std::endl;
    //std::cout << *p <<std::endl;


    std::array<int, 10> test_array  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::array<int, 10> test_array2 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    std::swap(test_array, test_array2);
    
    //std::cout << *p << std::endl;

    /*
    std::cout << "first array" << std::endl;

    for(auto a : test_array)
        std::cout << a << " ";

    */

    std::array<int, 10>::iterator it = test_array.begin();
    std::array<int, 10>::iterator it2 = test_array.begin();

    *it = 5;

    for(auto a : test_array)
    {
        std::cout << a << " ";
    }

    std::cout << std::endl;

    std::string str("123");


    std::cout << std::hex << "0x" << str.begin() << std::endl;
    std::cout << std::hex << "0x" << str.end() << std::endl;


    std::cout << std::endl;


    wait_key(ENTER_RELEASE);
    wait_key(ENTER_RELEASE);

}


extern "C"
{

    void cpp_test(void)
    {
        cpp_prog();
    }
    

}
