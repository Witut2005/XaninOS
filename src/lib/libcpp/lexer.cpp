
#include "./lexer.hpp"
#include <lib/libcpp/algorithm.h>
#include <sys/devices/com/com.h>

namespace std {

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

std::string BaseLexer::consume(uint32_t count)
{
    auto input_len = m_input.length();

    std::string(m_input.c_str() + m_index, count + m_index > input_len ? input_len : count);
    m_index = count + m_index > input_len ? input_len : m_index = count;
}

void BaseLexer::ignore(uint32_t count)
{
    m_index = std::min<uint32_t>(m_index + count, m_input.length());
}

std::string BaseLexer::consume_until(std::string end, bool ignore_end)
{
    if (m_index == m_input.length()) {
        return "";
    }

    auto endlen = end.length();
    auto start = m_input.cbegin() + m_index;

    for (auto it = start; it != m_input.cend(); it++, m_index++)
    {
        if (std::string(it, it + endlen) == end) {
            if (ignore_end) {
                ignore(endlen);
            }
            return std::string(start, it);
        }
    }

    return std::string(start, m_input.cend());
}

std::pair<std::string, std::string> BaseLexer::consume_until(std::vector<std::string> ends, bool ignore_end)
{
    if (m_index == m_input.length()) {
        return { "", "" };
    }

    auto start = m_input.cbegin() + m_index;

    for (auto it = start; it != m_input.cend(); it++, m_index++)
    {
        if (m_index == m_input.length()) {
            break;
        }

        for (auto end : ends) {
            auto endlen = end.length();

            if (std::string(it, it + endlen) == end) {
                if (ignore_end) {
                    ignore(endlen);
                }
                return { std::string(start, it), end };
            }
        }
    }

    return { std::string(start, m_input.cend()), "" };
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