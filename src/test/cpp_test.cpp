
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

    std::string x = "hello\n";



    std::cin >> x;

    screen_clear();

    x = "omglo";
    
    delete &x;

    std::string* tmp_s = (std::string*)malloc(sizeof(std::string));
    //memcpy((uint8_t*)&tmp_s, (uint8_t*)&std::base, sizeof(std::base));


    wait_key(ENTER_RELEASE);
    wait_key(ENTER_RELEASE);

}

}
