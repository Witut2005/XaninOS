
#include <lib/libcpp/string.h>
#include <lib/libcpp/container/vector.hpp>

namespace std {

class BaseLexer {
public:

    static constexpr int npos = -1;

    BaseLexer(const std::string& input)
    {
        m_input = input;
    }

    void reassign(const std::string& input);

    std::string rest(bool to_right = true);
    std::string consume(uint32_t count = 1);
    void ignore(uint32_t count = 1);
    std::string consume_until(std::string end, bool ignore_end = true);
    std::pair<std::string, std::string> consume_until(std::vector<std::string> ends, bool ignore_end = false);
    void ignore_until(std::string end);
    std::string str_get(void);

    uint32_t index_get(void);
    void index_set(uint32_t new_index);
    bool all_parsed(void);

private:
    std::string m_input;
    uint32_t m_index{ 0 };
};

} // namespace
