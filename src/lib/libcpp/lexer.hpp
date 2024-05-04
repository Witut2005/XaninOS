
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

    BaseLexer(const string& input) : m_input(input) {}
    BaseLexer(const BaseLexer&) = default;

    void reassign(const string& input);
    string rest(bool to_right = true);

    char peek(void);
    string consume(uint32_t count = 1);
    void ignore(uint32_t count = 1);

    std::string consume_until(std::string end, bool ignore_end = true);

    template<typename T = bool(*)(const string&) >
    std::pair<string, string> consume_until(std::vector<string> ends, bool ignore_end = true, T exception = [](const string&) {return true;});

    void ignore_until(string end);

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

template<typename T>
std::pair<string, string> BaseLexer::consume_until(std::vector<string> ends, bool ignore_end, T exception)
{
    if (m_index == m_input.length()) {
        return { "", "" };
    }

    auto start = m_input.cbegin() + m_index;

    for (auto it = start; it != m_input.cend(); it++, m_index++)
    {
        for (const auto& end : ends) {
            auto endlen = end.length();
            // dbg_info("HUJ", xsprintf("             ", "%d", endlen));

            auto str2chk = string(it, it + endlen);
            // dbg_info("HUJ", str2chk.c_str());

            if (exception_consume(str2chk == end) && exception(str2chk))
            {
                if (ignore_end) {
                    ignore(endlen);
                }
                return { string(start, it), end };
            }
        }
    }

    return { string(start, m_input.cend()), "" };
}

} // namespace
