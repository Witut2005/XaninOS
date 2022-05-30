
#include <libcpp/cstring.h>
#include <libcpp/cmemory.h>
#include <libcpp/ostream.h>
#include <libcpp/cstdlib.h>
#include <libcpp/string.h>
#include <libcpp/istream.h>
#include <keyboard/scan_codes.h>
#include <libcpp/array.cpp>
#include <libcpp/initializer_list.hpp>

extern "C"
{

void cpp_test(void)
{

    std::string x;
    x = "omglo";
    std::cin >> x;

    std::cout << std::clear;

    std::array<int, 10> test_array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for(auto a : test_array)
    {
        std::cout << a << std::endl;
    }

    wait_key(ENTER_RELEASE);
    wait_key(ENTER_RELEASE);

}

}
