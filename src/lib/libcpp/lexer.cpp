
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

    for (auto it = m_input.cbegin() + m_index; it != m_input.cend(); it++, m_index++)
    {
        if (exception_consume(std::string(it, it + endlen) == end)) return;
    }
}

std::string BaseLexer::consume_until(std::string end, IgnoreEnd ignore_end)
{
    if (m_index == m_input.length()) return "";

    auto endlen = end.length();
    auto start = m_input.cbegin() + m_index;

    for (auto it = start; it != m_input.cend(); it++, m_index++)
    {
        if (exception_consume(std::string(it, it + endlen) == end))
        {
            if (ignore_end == IgnoreEnd::Yes) {
                ignore(endlen);
            }
            return std::string(start, it);
        }
    }

    return std::string(start, m_input.cend());
}

bool BaseLexer::exception_consume(bool cond)
{
    if (cond && m_exceptions) { m_exceptions--; return false; }
    else if (cond && !m_exceptions) { return true; }
    else { return false; }
}

uint32_t BaseLexer::count_number_of_continuous_chars(int pos, uint32_t max_count)
{
    return count_number_of_continuous_chars(m_input[pos], pos, max_count);
}

uint32_t BaseLexer::count_number_of_continuous_chars(char char_expected, int pos, uint32_t max_count)
{
    if (m_input[pos] != char_expected) return 0;
    pos = m_input.index_serialize(pos);

    auto str = std::string(m_input.cbegin() + pos, m_input.length() > pos + max_count ? m_input.cbegin() + pos + max_count : m_input.cend());
    //std::is_overflow<OverflowCheck::Addition, unsigned int>(pos, max_count) ? m_input.cend() : m_input.cbegin() + pos + max_count);

    if (str.empty()) return 0;

    uint32_t count = 1;
    for (int i = 1; i < str.length(); i++)
    {
        if (char_expected == str[i]) {
            count++;
        }
        else break;
    }
    return count;
}

void BaseLexer::index_set(uint32_t new_index)
{
    auto input_len = m_input.length();
    m_index = new_index > input_len ? input_len : new_index;
}


} // namespace