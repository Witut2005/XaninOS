
#include "./lexer.hpp"
#include <lib/libcpp/algorithm.h>

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

std::string BaseLexer::consume_until(std::string end, uint32_t max_count)
{

}

void BaseLexer::ignore_until(std::string end, uint32_t max_count)
{
    //dopoki std::string(m_input.c_str() + m_index, count) != end
}