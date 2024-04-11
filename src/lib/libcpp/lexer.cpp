
#include "./lexer.hpp"
#include <lib/libcpp/algorithm.h>
#include <sys/devices/com/com.h>

//check if '\0'

std::string BaseLexer::consume(uint32_t count)
{
    //std::string(m_input.c_str() + m_index, count);
    //m_index++;
}

void BaseLexer::ignore(uint32_t count)
{
    m_index = std::min<uint32_t>(m_index + count, m_input.length());
}

std::string BaseLexer::consume_until(std::string end)
{
    auto len = m_input.length();
    auto endlen = end.length();

    // for (auto& it : m_input)
    for (auto it = m_input.begin(); it != m_input.end(); it++)
    {
        if (std::string(it, it + endlen) == end)
            dbg_success(DEBUG_LABEL_LEXER, "Hurray");
    }

    dbg_warning(DEBUG_LABEL_LEXER, "Not found");
}

void BaseLexer::ignore_until(std::string end)
{
    //dopoki std::string(m_input.c_str() + m_index, count) != end
}