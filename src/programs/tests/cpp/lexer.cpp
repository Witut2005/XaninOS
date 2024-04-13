
#include <lib/libcpp/lexer.hpp>
#include <lib/libcpp/string.h>
#include <lib/libcpp/utility.h>

extern "C" __STATUS __cpp_lexer_test(void)
{

    constexpr const char* str = "nicho";

    std::BaseLexer lexer(str);
    xprintf("result: %s\n", (lexer.consume_until("cho").c_str()));
    xprintf("result: %s\n", (lexer.str_get().c_str()));
    // xprintf("result: %d\n", bstrcmp(lexer.consume_until("cho").c_str(), str));
}
