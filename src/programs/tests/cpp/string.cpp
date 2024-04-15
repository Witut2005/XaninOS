
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>

using namespace std;
using namespace std::literals;

extern "C" __STATUS __cpp_string_test(void)
{
    constexpr char* test_str = "nicho";
    constexpr char* reversed_test_str = "ohcin";

    std::string nicho = test_str;

    print("iterator constructor: %d\n", nicho == std::string(nicho.begin(), nicho.end()));
    print("riterator constructor: %d\n", std::string(nicho.rbegin(), nicho.rend()) == std::string(reversed_test_str));

    print("str + str: %s\n", (nicho + nicho).c_str());

    std::string copytome = nicho;

    print("constructors: %d\n", nicho == std::string(test_str));


    print("first of %s: %d\n", nicho.c_str(), nicho.first_of("ch") == 2);
    print("last of %s: %d\n", nicho.c_str(), nicho.last_of("ni") == 0);

    std::string movetome(std::move(nicho));
    print("move operator = %d\n", !(movetome == nicho)); // should be false

    const std::string cons("ugauga");

    print("str == str: {}\n", "abc"s == "abc"s);
    print("str < str: {}\n", "abc"s < "abcd"s);
    print("str > str: {}\n", !("abc"s > "abcd"s));
    print("str <= str: {}\n", "abc"s <= "abc"s);
    print("str >= str: {}\n", "abc"s >= "abc"s);

    print("--------------std::string_view--------------\n");

    print("str == str: {}\n", "abc"sv == "abc"sv);
    print("str < str: {}\n", "abc"sv < "abcd"sv);
    print("str > str: {}\n", !("abc"sv > "abcd"sv));
    print("str <= str: {}\n", "abc"sv<= "abc"sv);
    print("str >= str: {}\n", "abc"sv >= "abc"sv);

    cons + cons;


    return XANIN_OK;
}