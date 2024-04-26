

#include <lib/libcpp/ostream.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>
#include <programs/tests/test_case.h>
#include <sys/devices/com/com.h>

using namespace std;
using namespace std::literals;

#warning "TODO NULL str initialization is sus";
extern "C" __STATUS __cpp_string_test(void)
{
    constexpr char* test_str = "nicho";
    constexpr char* reversed_test_str = "ohcin";

    TEST_CASE(constructors)
    {
        std::string nicho = test_str;
        auto nicho2 = string(nicho.begin(), nicho.end());
        EXPECT_EQUAL(string(nicho), "nicho");
        EXPECT_EQUAL(string("nicho", 3), "nic");
        EXPECT_EQUAL(string("nicho"), "nicho");
        EXPECT_EQUAL(string(nicho.begin(), nicho.end()), "nicho");
        EXPECT_EQUAL(string(nicho.begin() + 1, nicho.end() - 1), "ich");
        EXPECT_EQUAL(string(nicho.rbegin(), nicho.rend()), "ohcin");
        EXPECT_EQUAL(string(nicho.rbegin() + 1, nicho.rend() - 1), "hci");
    }

    TEST_CASE(size modification)
    {
        std::string nicho = test_str;
        nicho.push_back('o');
        EXPECT_EQUAL(string(nicho), "nichoo");
        nicho.pop_back();
        EXPECT_EQUAL(string(nicho), "nicho");
        auto inserter = std::back_inserter(nicho);
        inserter('c');
        EXPECT_EQUAL(string(nicho), "nichoc");
    }



    // print("iterator constructor: {}\n", string(nicho.begin(), nicho.end() - 1));
    // print("iterator constructor: {} {}\n", nicho, nicho == string(nicho.begin(), nicho.end()));
    // print("tricky iterator constructor: {}\n", string(nicho.begin(), nicho.begin()));
    // print("riterator constructor: %d\n", string(nicho.rbegin(), nicho.rend()) == string(reversed_test_str));

    // print("str + str: %s\n", (nicho + nicho).c_str());

    // std::string copytome = nicho;

    // print("constructors: %d\n", nicho == std::string(test_str));
    // print("first of {}: %d\n", nicho, nicho.first_of("ch") == 2);
    // print("last of {}: %d\n", nicho, nicho.last_of("nic") == 0);
    // print("operator + (char) {}\n", nicho + 'x' == "nichox");

    // auto res_str = string(nicho);
    // res_str.resize(7, 'o');
    // print("resize {}\n", res_str);
    // print("last of (second test) %s: %d\n", nicho.c_str(), nicho.last_of("i", -3) == 1);
    // print("last of (second test) %s: %d\n", nicho.c_str(), nicho.last_of("i", 2) == 1);
    // print("last of (second test) %s: %d\n", nicho.c_str(), nicho.index_serialize(-3) == 2);

    // print("substr: {}\n", nicho.substr(1, 3).c_str());// == "icho"s);
    // print("substr: {}\n", nicho.substr(0, 3).c_str());//== "nic"s);

    // std::string movetome(std::move(nicho));
    // print("move operator = %d\n", !(movetome == nicho)); // should be false

    // const std::string cons("ugauga");

    // print("str == str: {}\n", "abc"s == "abc"s);
    // print("str < str: {}\n", "abc"s < "abcd"s);
    // print("str > str: {}\n", !("abc"s > "abcd"s));
    // print("str <= str: {}\n", "abc"s <= "abc"s);
    // print("str >= str: {}\n", "abc"s >= "abc"s);

    // print("--------------std::string_view--------------\n");

    // print("str == str: {}\n", "abc"sv == "abc"sv);
    // print("str < str: {}\n", "abc"sv < "abcd"sv);
    // print("str > str: {}\n", !("abc"sv > "abcd"sv));
    // print("str <= str: {}\n", "abc"sv <= "abc"sv);
    // print("str >= str: {}\n", "abc"sv >= "abc"sv);

    // cons + cons;


    return XANIN_OK;
}