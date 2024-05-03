
#pragma once

#include <lib/libc/string.h>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/container/iterator.hpp>
#include <sys/devices/com/com.h>

namespace std {

class string {
public:
    using value_type = char;

    using iterator = RandomAccessIterator<string>;
    using const_iterator = ConstRandomAccessIterator<string>;
    using reversed_iterator = RandomAccessReversedIterator<string>;
    using const_reversed_iterator = ConstRandomAccessReversedIterator<string>;

    string(void);
    explicit string(uint32_t size);

    template<typename It>
        requires is_same<It, string::iterator>::value || is_same<It, string::const_iterator>::value
    string(It beg, It end);

    template<typename It>
        requires is_same<It, string::reversed_iterator>::value || is_same<It, string::const_reversed_iterator>::value
    string(It beg, It end);

    string(char const* other, uint32_t size);

    string(char const* str);
    string(string const& str);
    string(string&& str);
    ~string();

    int index_serialize(int index) const;
    uint32_t capacity(void) const; // returns m_size_reserved - sizeof('\0')
    void reserve(uint32_t size);   // reserves to hold size characters
    const string& resize(uint32_t size, char c = '\0');
    constexpr char const* c_str(void) const { return m_ptr; }

    constexpr char* data(void) { return m_ptr; }
    constexpr const char* data(void) const { return m_ptr; }

    bool empty(void) const { return length() == 0; }

    uint32_t length(void) const;
    uint32_t size(void) const;
    void clear(void);
    void push_back(char c);
    char pop_back(void);

    string substr(int start_index, size_t len = npos) const;
    int last_of(const string& to_find, int start_index = -1) const;
    int first_of(const string& to_find, int start_index = 0) const;

    operator bool() const;
    int operator<=>(string const& other) const { return strcmp(m_ptr, other.m_ptr); }
    char& operator[](int index);
    char const& operator[](int index) const;

    string& operator=(string const& other);
    string& operator=(string&& other);
    string operator+(char c) const;
    string operator+(string const& other) const;
    bool operator==(string const& other) const;
    bool operator!=(string const& other) const;

    // NDEFINE_CLASS_RANGE_OPERATIONS(string);
    iterator begin(void) { return iterator(*this, 0); }
    iterator end(void) { return iterator(*this, size()); }
    const_iterator cbegin(void) const { return const_iterator(*this, 0); }
    const_iterator cend(void) const { return const_iterator(*this, size()); }

    reversed_iterator rbegin(void) { return reversed_iterator(*this, size() - 1); }
    reversed_iterator rend(void) { return reversed_iterator(*this, -1); }
    const_reversed_iterator crbegin(void) const { return const_reversed_iterator(*this, size() - 1); }
    const_reversed_iterator crend(void) const { return const_reversed_iterator(*this, -1); }

    static constexpr int npos = -1;

private:
    uint32_t reallocate_if_needed(uint32_t size); // returns true when data was reallocted

    char* m_ptr{ nullptr };
    uint32_t m_size_reserved{ 0 };
};

template<typename It>
concept StringIt = requires() {
    is_same<It, string::iterator>::value || is_same<It, string::reversed_iterator>::value
        || is_same<It, string::const_iterator>::value || is_same<It, string::const_reversed_iterator>::value;
};

class string_view {
public:
    using value_type = char;
    using iterator = string::const_iterator;
    using const_iterator = string::const_iterator;
    using reversed_iterator = string::const_reversed_iterator;
    using const_reversed_iterator = string::const_reversed_iterator;

    constexpr string_view() = delete;
    constexpr string_view(nullptr_t) = delete;
    string_view(char const* str)
        : m_ptr(str)
        , m_size(strlen(str))
    {
    }
    string_view(string str)
        : m_ptr(str.c_str())
        , m_size(str.length())
    {
    }
    constexpr string_view(char const* str, size_t size)
        : m_ptr(str)
        , m_size(size)
    {
    }
    template <StringIt It>
    string_view(It beg);

    constexpr uint32_t length(void) const { return m_size; }
    constexpr uint32_t size(void) { return m_size; }

    constexpr operator bool() { return m_ptr != nullptr; }
    int operator<=>(string_view const& other) { return strcmp(m_ptr, other.m_ptr); }
    const constexpr char& operator[](int index) { return m_ptr[index]; }

    string_view& operator=(string_view const& other)
    {
        m_ptr = other.m_ptr;
        m_size = other.m_size;
        return *this;
    }
    string_view& operator=(string_view&& other)
    {
        m_ptr = other.m_ptr;
        m_size = other.m_size;
        other.m_ptr = nullptr;
        return *this;
    }

    bool operator==(string_view const& other) { return bstrcmp(m_ptr, other.m_ptr); }
    bool operator!=(string const& other) { return !(*this == other); }

    // const_iterator begin(void) { return const_iterator(m_ptr); }
    // const_iterator end(void) { return const_iterator(m_ptr + m_size); }
    // const_iterator cbegin(void) { return const_iterator(m_ptr); }
    // const_iterator cend(void) { return const_iterator(m_ptr + m_size); }

    // const_reversed_iterator rbegin(void) { return const_reversed_iterator(*this, ); }
    // const_reversed_iterator rend(void) { return const_reversed_iterator(m_ptr - 1); }
    // const_reversed_iterator crbegin(void) { return const_reversed_iterator(m_ptr + m_size - 1); }
    // const_reversed_iterator crend(void) { return const_reversed_iterator(m_ptr - 1); }

private:
    // use here std::string instead of ptr
    char const* m_ptr;
    size_t m_size;
};

// ITERATOR CONSTRUCTOR
template<typename It>
    requires is_same<It, string::iterator>::value || is_same<It, string::const_iterator>::value
string::string(It beg, It end) : string()
{
    if (beg < end)
    {
        uint32_t size_to_allocate = end.data() - beg.data();
        reallocate_if_needed(size_to_allocate);
        strncpy(m_ptr, beg.data(), size_to_allocate);
    }
}

// RITERATOR CONSTRUCTOR
template<typename It>
    requires is_same<It, string::reversed_iterator>::value || is_same<It, string::const_reversed_iterator>::value
string::string(It beg, It end) : string()
{
    if (beg < end)
    {
        uint32_t size_to_allocate = beg.data() - end.data();
        reallocate_if_needed(size_to_allocate);

        int i = 0;
        for (auto it = beg.data(); it != end.data(); it--, i++) {
            m_ptr[i] = *it;
        }
        m_ptr[i] = '\0';
    }
}

template<StringIt It>
string_view::string_view(It beg)
    : m_ptr(beg.data())
{
    for (int i = 0; m_ptr[i] != '\0'; i++)
    {
        m_size++;
    }
}

namespace literals {

static inline string operator""s(char const* str, size_t len)
{
    return std::string(str, len);
}

constexpr static inline string_view operator""sv(char const* str, size_t len)
{
    return std::string_view(str, len);
}

}

} // namespace
