
#include <lib/libcpp/string.h>
#include <lib/libcpp/container/vector.hpp>

namespace std {

// template <typename T>
// class AlwaysTrue {
// public:
//     operator bool(T arg)() { return true; }
// }

class BaseLexer {
public:
    static constexpr int npos = -1;

    BaseLexer(const std::string& input) : m_input(input) {}

    void reassign(const std::string& input);
    std::string rest(bool to_right = true);

    char peek(void);
    std::string consume(uint32_t count = 1);
    void ignore(uint32_t count = 1);

    template <typename T = bool(*)(std::string)>
    std::string consume_until(std::string end, bool ignore_end = true, T predicate = [](std::string a) {return true;});

    template <typename T = bool(*)(std::string)>
    std::pair<std::string, std::string> consume_until(std::vector<std::string> ends, bool ignore_end = true, T predicate = [](std::string a) {return true;});

    void ignore_until(std::string end);

    std::string str_get(void);
    uint32_t index_get(void);
    void index_set(uint32_t new_index);

    bool all_parsed(void);
private:
    std::string m_input;
    uint32_t m_index{ 0 };
};

////////////////////Methods definitions//////////////////////////

template <typename T>
std::string BaseLexer::consume_until(std::string end, bool ignore_end, T predicate)
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

template <typename T>
std::pair<std::string, std::string> BaseLexer::consume_until(std::vector<std::string> ends, bool ignore_end, T predicate)
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

        for (const auto& end : ends) {
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


// std::string BaseLexer::conditional_consume_until(auto predicate, std::string end, bool ignore_end)
// {
//     if (m_index == m_input.length()) {
//         return "";
//     }

//     auto endlen = end.length();
//     auto start = m_input.cbegin() + m_index;

//     for (auto it = start; it != m_input.cend(); it++, m_index++)
//     {
//         if (std::string(it, it + endlen) == end) {
//             if (ignore_end) {
//                 ignore(endlen);
//             }
//             return std::string(start, it);
//         }
//     }

//     return std::string(start, m_input.cend());
// }

// std::pair<std::string, std::string> BaseLexer::conditional_consume_until(auto predicate, std::vector<std::string> ends, bool ignore_end)
// {
//     if (m_index == m_input.length()) {
//         return { "", "" };
//     }

//     auto start = m_input.cbegin() + m_index;

//     for (auto it = start; it != m_input.cend(); it++, m_index++)
//     {
//         if (m_index == m_input.length()) {
//             break;
//         }

//         for (const auto& end : ends) {
//             auto endlen = end.length();

//             if (std::string(it, it + endlen) == end) {
//                 if (ignore_end) {
//                     ignore(endlen);
//                 }
//                 return { std::string(start, it), end };
//             }
//         }
//     }

//     return { std::string(start, m_input.cend()), "" };
// }


} // namespace
