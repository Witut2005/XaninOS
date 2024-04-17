
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
    using value_type = char;
    using Type = char;
    using Iterator = NStringIterator;
    using ReversedIterator = ConstReversedNStringIterator;
    using ConstIterator = ConstNStringIterator;
    using ConstReversedIterator = ConstReversedNStringIterator;

    string(void);
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

    int index_serialize(int index) const;
    uint32_t capacity(void) const; //returns m_size_reserved - sizeof('\0')
    void reserve(uint32_t size); //reserves to hold size characters
    const char* c_str(void) const;
    uint32_t length(void) const;
    uint32_t size(void) const;
    void clear(void);

    string substr(int start_index, size_t len = npos) const;
    int last_of(std::string to_find, int start_index = -1) const;
    int first_of(std::string to_find, int start_index = 0) const;

    operator bool() const;
    int operator <=> (const string& other) const { return strcmp(m_ptr, other.m_ptr); }
    char& operator[](int index);
    const char& operator[](int index) const;

    string& operator=(string const& other);
    string& operator=(std::string&& other);
    string operator+(const std::string& other) const;
    bool operator == (string const& other) const;
    bool operator != (string const& other) const;

    DEFINE_CLASS_RANGE_OPERATIONS(NStringIterator);

    static constexpr int npos = -1;

private:
    bool reallocate_if_needed(uint32_t size);     // returns true when data was reallocted

    char* m_ptr;
    uint32_t m_size_reserved{ 0 };
};

class string_view {
public:
    using value_type = char;
    using Type = char;
    using Iterator = NStringIterator;
    using ReversedIterator = ConstReversedNStringIterator;
    using ConstIterator = ConstNStringIterator;
    using ConstReversedIterator = ConstReversedNStringIterator;

    constexpr string_view() = delete;
    constexpr string_view(nullptr_t) = delete;
    string_view(const char* str) : m_ptr(str), m_size(strlen(str)) {}
    string_view(string str) : m_ptr(str.c_str()), m_size(str.length()) {}
    constexpr string_view(const char* str, size_t size) : m_ptr(str), m_size(size) {}
    template <StringIt It>
    string_view(It beg);

    constexpr uint32_t length(void) const { return m_size; }
    constexpr uint32_t size(void) { return m_size; }

    constexpr operator bool() { return m_ptr != nullptr; }
    int operator <=> (const string_view& other) { return strcmp(m_ptr, other.m_ptr); }
    const constexpr char& operator[](int index) { return m_ptr[index]; }

    string_view& operator=(string_view const& other) { m_ptr = other.m_ptr; m_size = other.m_size; return *this; }
    string_view& operator=(string_view&& other) {
        m_ptr = other.m_ptr;
        m_size = other.m_size;
        other.m_ptr = nullptr;
        return *this;
    }

    bool operator == (string_view const& other) { return bstrcmp(m_ptr, other.m_ptr); }
    bool operator != (string const& other) { return !(*this == other); }

    ConstIterator begin(void) { return ConstIterator(m_ptr); }
    ConstIterator end(void) { return ConstIterator(m_ptr + m_size); };
    ConstReversedIterator rbegin(void) { return ConstReversedIterator(m_ptr + m_size - 1); };
    ConstReversedIterator rend(void) { return ConstReversedIterator(m_ptr - 1); };

    ConstIterator cbegin(void) { return ConstIterator(m_ptr); }
    ConstIterator cend(void) { return ConstIterator(m_ptr + m_size); };
    ConstReversedIterator crbegin(void) { return ConstReversedIterator(m_ptr + m_size - 1); };
    ConstReversedIterator crend(void) { return ConstReversedIterator(m_ptr - 1); };

private:
    const char* m_ptr;
    size_t m_size;
};

template <StringIt It>
string::string(It beg, It end) : string()
{
    if (beg.data() < end.data())
    {
        auto size_to_allocate = (uint32_t)end.data() - (uint32_t)beg.data();
        reallocate_if_needed(size_to_allocate);
        memcpy(m_ptr, beg.data(), size_to_allocate);
    }
}
template <StringIt It>
string_view::string_view(It beg) : m_ptr(beg.data())
{
    for (int i = 0; m_ptr[i] != '\0'; i++) {
        m_size++;
    }
}

namespace literals
{

static inline string operator""s(const char* str, size_t len)
{
    return std::string(str, len);
}

constexpr static inline string_view operator""sv(const char* str, size_t len)
{
    return std::string_view(str, len);
}

}

}// namespace


