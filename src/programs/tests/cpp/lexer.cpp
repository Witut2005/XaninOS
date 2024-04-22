
#include <lib/libc/stdiox.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/lexer.hpp>
#include <lib/libcpp/utility.h>

extern "C" __STATUS __cpp_lexer_test(void)
{

    constexpr const char* str = "nicho";

    std::BaseLexer lexer(str);
    xprintf("result: %s\n", (lexer.consume_until("cho").c_str()));
    xprintf("result: %s\n", (lexer.str_get().c_str()));

    lexer.reassign("/etc/config/../files/./nicho.txt");

    std::string path;

    while (lexer.all_parsed() == false)
    {
        auto result = lexer.consume_until(std::vector<std::string>({ "../", "./" }), true);
        path = path + result.first;
        if (result.second == "../") {
            path = std::string(path.begin(), path.begin() + path.last_of("/", -2));
        }
        xprintf("parsed path: %s delim: %s\n", path.c_str(), result.second.c_str());
    }

    lexer.reassign("123456");
    lexer.ignore(2);
    xprintf("rest test: %s\n", lexer.rest(false).c_str());
    xprintf("rest test: %s\n", lexer.rest(true).c_str());

    return XANIN_OK;

    // if (result.second == "../") {
    //     result.first = std::string(result.first.begin(), result.first.begin() + result.first.last_of("/", -2));
    // }

    // xprintf("stage2: %s\n", result.first.c_str());

    // result.first = result.first + lexer.consume_until(std::vector<std::string>({ "../", "./" }), true).first;
    // xprintf("stage3: %s\n", result.first.c_str());

    // xprintf("result: %d\n", bstrcmp(lexer.consume_until("cho").c_str(), str));
}
