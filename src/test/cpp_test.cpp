

#include <libcpp/ostream.h>
#include <libcpp/cstdlib.h>
#include <libcpp/string.h>
#include <libcpp/istream.h>
#include <keyboard/scan_codes.h>

extern "C"
{

void cpp_test(void)
{

    std::string x = "hello\n";

    xprintf("%x\n", &x.string_data);
    xprintf("%x\n", &x.c_str());


    std::cin >> x;

    screen_clear();

    xprintf("%s\n", x.c_str());

    wait_key(ENTER_RELEASE);
    wait_key(ENTER_RELEASE);

}

}