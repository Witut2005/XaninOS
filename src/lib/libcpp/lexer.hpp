
#include <lib/libcpp/string.h>

class BaseLexer
{
public:
    BaseLexer(std::string input) : m_input(input) {}
    std::string consume(uint32_t count = 1);
    void ignore(uint32_t count = 1);
    std::string consume_until(std::string end, uint32_t max_count = UINT32_MAX);
    void ignore_until(std::string end, uint32_t max_count = UINT32_MAX);

private:
    std::string m_input;
uint32_t m_index{ 0 };
};