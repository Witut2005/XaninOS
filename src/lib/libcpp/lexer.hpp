
#include <lib/libcpp/string.h>

namespace std {

class BaseLexer {
public:
    BaseLexer(std::string input)
        : m_input(input)
    {
    }
    std::string consume(uint32_t count = 1);
    void ignore(uint32_t count = 1);
    std::string consume_until(std::string end);
    void ignore_until(std::string end);
    std::string str_get(void);

private:
    std::string m_input;
    uint32_t m_index { 0 };
};

} // namespace
