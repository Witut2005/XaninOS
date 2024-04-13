
#include "./lexer.hpp"
#include <lib/libcpp/algorithm.h>
#include <sys/devices/com/com.h>

namespace std {

// check if '\0'
std::string BaseLexer::consume(uint32_t count)
{
    // std::string(m_input.c_str() + m_index, count);
    // m_index++;
}

void BaseLexer::ignore(uint32_t count)
{
    m_index = std::min<uint32_t>(m_index + count, m_input.length());
}

#warning "TODO m_index update";
std::string BaseLexer::consume_until(std::string end)
{
    auto endlen = end.length();
    auto start = m_input.begin() + m_index;

    for (auto it = start; it != m_input.end(); it++)
    {
        auto tmp = std::string(it, it + endlen);
        dbg_info(DEBUG_LABEL_LEXER, tmp.c_str());

        if (tmp == end) {
            dbg_success(DEBUG_LABEL_LEXER, std::string(start, it).c_str());
            return std::string(start, it);
        }
    }

    return std::string("");
}

void BaseLexer::ignore_until(std::string end)
{
    // dopoki std::string(m_input.c_str() + m_index, count) != end
}

std::string BaseLexer::str_get(void)
{
    return m_input;
}

} // namespace