
#include <libcpp/cstring.h>
#include <libcpp/cmemory.h>
#include <libcpp/ostream.h>
#include <libcpp/cstdlib.h>
#include <libcpp/string.h>
#include <libcpp/istream.h>
#include <keyboard/scan_codes.h>

extern "C"
{

void cpp_test(void)
{

    std::string x;
    x = "omglo";
    std::cin >> x;

    std::cout << std::clear;

    std::cout << x << '\n'; 
    std::cout << x << '\n'; 

    wait_key(ENTER_RELEASE);
    wait_key(ENTER_RELEASE);

}

}
