
// #include <devices/NIC/ne2000.hpp>
#include <libcpp/stack.h>
#include <libcpp/utility.h>
#include <libcpp/time.hpp>
#include <libcpp/algorithm.h>
#include <libcpp/cstring.h>
#include <libcpp/cmemory.h>
#include <libcpp/ostream.h>
#include <libcpp/cstdlib.h>
#include <libcpp/string.h>
#include <libcpp/istream.h>
#include <keyboard/scan_codes.h>
#include <libcpp/array.h>
#include <libcpp/initializer_list.hpp>
#include <libcpp/new.hpp>
#include <libcpp/box.h>
#include <libcpp/iterator.h>
#include <libcpp/bytes.h>

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

    std::cout << "the smallest element: " << *std::min_element(test_array.begin(), test_array.end()) << std::endl;

    std::cout << "the largest element: " << *std::max_element(test_array.begin(), test_array.end()) << std::endl;
    
    std::swap<int>(*test_array.begin(), *test_array.end());
    std::min(*test_array.begin(), *test_array.end());
    
    std::string str_tmp = "hm";
    std::cout << str_tmp << std::endl;
    

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
    std::cout << "STRING SIZE:"<< str.size() << std::endl;


    // std::cout << std::hex << "0x" << str.begin() << std::endl;
    // std::cout << std::hex << "0x" << str.end() << std::endl;


    std::cout << std::endl;

    std::pair<char, int> para = std::make_pair('a', 512); 
    std::pair<char, int>& ref_pair = para;

    std::cout << para.first << " " << para.second << std::endl;

    std::stack<int> stack;

    for(int i = 0; i < 5; i++)
        stack.push(i);

    while(!stack.empty())
    {
        std::cout << stack.top() << std::endl;
        stack.pop();
    }

    int first = 100, second = 50;

    std::pair pair_test = std::minmax(first, second);

    pair_test.first = 10;
    pair_test.second = 5;

    std::cout << std::dec << first << " " << second << std::endl;

    std::cout << "string test" << std::endl;


    std::string str1 = "jeden";


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
