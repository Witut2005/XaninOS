
#pragma once

#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libcpp/container/iterator.hpp>
#include <lib/libcpp/type_traits.h>

namespace std {

RANDOM_ACCESS_ITERATORS_DECLARE(NStringIterator, char)

template <typename T>
concept StringIt = requires() {
    is_same<T, NStringIterator>::value || is_same<T, ReversedNStringIterator>::value
        || is_same<T, ConstNStringIterator>::value || is_same<T, ConstReversedNStringIterator>::value;
};

class string {
public:
    using Type = char;
    using Iterator = NStringIterator;
    using ReversedIterator = ConstReversedNStringIterator;
    using ConstIterator = ConstNStringIterator;
    using ConstReversedIterator = ConstReversedNStringIterator;
    static constexpr int npos = -1;

    string(void) = default;
    explicit string(uint32_t size);

    // string(const NStringIterator beg, const NStringIterator end);
    template <StringIt It>
    string(It beg, It end);

    // template <typename Str, typename Size>
    string(char const* other, uint32_t size);

    string(char const* str);
    string(string const& str);
    string(string&& str);
    ~string(void);

    void reserve(uint32_t size); //reserves to hold size characters
    char const* c_str(void) const;
    uint32_t capacity(void) const; //returns m_size_reserved - sizeof('\0')
    uint32_t length(void) const;
    uint32_t size(void) const;

    int last_of(std::string to_find, int start_index = -1) const;
    int first_of(std::string to_find, int start_index = 0) const;

    char& operator[](uint32_t index);
    const char& operator[](uint32_t index) const;

    // string& operator=(std::string other);
    string& operator=(string const& other);
    string& operator=(std::string&& other);
    string operator+(char character);
    string operator+(const std::string& other);
    bool operator == (string const& other);
    bool operator != (string const& other);

    DEFINE_CLASS_RANGE_OPERATIONS(NStringIterator);

private:
    bool reallocate_if_needed(uint32_t size);     // returns true when data was reallocted

    char* m_ptr{ nullptr };
    uint32_t m_size_reserved{ 0 };
};

template <StringIt It>
string::string(It beg, It end)
{
    int i = 1;
    for (;beg != end; beg++, i++) {
        reallocate_if_needed(i);
        m_ptr[i - 1] = *beg;
    }
    m_ptr[i - 1] = '\0';
}

} // namespace

