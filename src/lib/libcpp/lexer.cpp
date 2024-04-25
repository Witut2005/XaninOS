
#include "./lexer.hpp"
#include <lib/libcpp/algorithm.h>
#include <sys/devices/com/com.h>
#include <lib/libcpp/memory.hpp>

namespace std {

using namespace literals;

#warning "TODO unique copy";
// check if '\0'
void BaseLexer::reassign(const std::string& input)
{
    m_index = 0;
    m_input = input;
}

std::string BaseLexer::rest(bool to_right)
{
    return to_right ? std::string(m_input.begin() + m_index, m_input.end()) : std::string(m_input.begin(), m_input.begin() + (m_index + 1));
}

char BaseLexer::peek(void)
{
    m_index++;
    return *m_input.c_str();
}

std::string BaseLexer::consume(uint32_t count)
{
    auto input_len = m_input.length();

    auto tmp = std::string(m_input.c_str() + m_index, count + m_index > input_len ? input_len : count);
    m_index = count + m_index > input_len ? input_len : m_index = count;
    return tmp;
}

void BaseLexer::ignore(uint32_t count)
{
    m_index = std::min<uint32_t>(m_index + count, m_input.length());
}

void BaseLexer::ignore_until(std::string end)
{
    auto endlen = end.length();
    auto start = m_input.cbegin() + m_index;

    for (auto it = start; it != m_input.cend(); it++, m_index++) {
        if (std::string(it, it + endlen) == end) return;
    }
}

std::string BaseLexer::str_get(void)
{
    return m_input;
}

uint32_t BaseLexer::index_get(void)
{
    return m_index;
}

void BaseLexer::index_set(uint32_t new_index)
{
    auto input_len = m_input.length();
    m_index = new_index > input_len ? input_len : new_index;
}

bool BaseLexer::all_parsed(void)
{
    return !(m_index < m_input.length());
}

} // namespace