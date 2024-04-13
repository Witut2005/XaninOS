
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>

extern "C" __STATUS __cpp_string_test(void)
{
    constexpr char* test_str = "nicho";
    constexpr char* reversed_test_str = "ohcin";

    std::string nicho = test_str;

    xprintf("c_str: %s\n", nicho.c_str());
    xprintf("capacity: %d\n", nicho.capacity());
    xprintf("bstrcmp: %d\n", bstrcmp(nicho.c_str(), test_str));
    xprintf("strlen: %d\n", strlen(nicho.c_str()) == strlen(test_str));
    xprintf("str == str: %d\n", nicho == std::string(test_str));
    xprintf("str != str: %d\n", !(nicho != std::string(test_str)));

    xprintf("iterator constructor: %d\n", nicho == std::string(nicho.begin(), nicho.end()));
    xprintf("riterator constructor: %d\n", std::string(nicho.rbegin(), nicho.rend()) == std::string(reversed_test_str));

    xprintf("str + str: %s\n", (nicho + nicho).c_str());

    std::string copytome;
    copytome = nicho;
    xprintf("copy operator = %d\n", copytome == nicho);

    xprintf("first of %s: %d\n", nicho.c_str(), nicho.first_of("ch") == 2);
    xprintf("last of %s: %d\n", nicho.c_str(), nicho.last_of("ni") == 0);

    std::string movetome(std::move(nicho));
    xprintf("move operator = %d\n", !(movetome == nicho)); // should be false


    return XANIN_OK;
}