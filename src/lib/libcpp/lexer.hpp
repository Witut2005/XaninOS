
#include <lib/libcpp/string.h>
#include <lib/libcpp/limits.hpp>
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

    std::string consume_until(std::string end, bool ignore_end = true);
    std::pair<std::string, std::string> consume_until(std::vector<std::string> ends, bool ignore_end = true);

    void ignore_until(std::string end);

    void exception_add(uint32_t number = 1) { m_exceptions += number; if (m_exceptions < 0) m_exceptions = 0; }
    // void exception_consume(uint32_t number = 1) { m_exceptions -= number; if (m_exceptions < 0) m_exceptions = 0; }
    bool exception_consume(bool cond); //returns true if cond && !m_execeptions
    void exception_reset(void) { m_exceptions = 0; }
    auto exceptions_get(void) { return m_exceptions; }

    uint32_t count_number_of_continuous_chars(int pos, uint32_t max_count = INT32_MAX); // doesnt change m_index position

    std::string str_get(void) { return m_input; }
    constexpr uint32_t index_get(void) { return m_index; }
    void index_set(uint32_t new_index);

    bool all_parsed(void) { return !(m_index < m_input.length()); }
private:
    std::string m_input;
    uint32_t m_index{ 0 };
    int m_exceptions{ 0 };
};

////////////////////Methods definitions//////////////////////////

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
