
#pragma once

#include <lib/libc/string.h>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/container/iterator.hpp>

namespace std {

// RANDOM_ACCESS_ITERATORS_DECLARE(NStringIterator, char);

class string {
public:
    using value_type = char;
    using Type = char;

    NRANDOM_ACCESS_ITERATORS_DECLARE(NStringIterator);

    using Iterator = NStringIterator<string>;
    using ReversedIterator = ReversedNStringIterator<string>;
    using ConstIterator = ConstNStringIterator<string>;
    using ConstReversedIterator = ConstReversedNStringIterator<string>;

    string(void);
    explicit string(uint32_t size);

    template<typename It>
        requires is_same<It, string::Iterator>::value || is_same<It, string::ConstIterator>::value
    string(It beg, It end);

    template<typename It>
        requires is_same<It, string::ReversedIterator>::value || is_same<It, string::ConstReversedIterator>::value
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
    bool empty(void) const { return length() == 0; }
    uint32_t length(void) const;
    uint32_t size(void) const;
    void clear(void);

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

    NDEFINE_CLASS_RANGE_OPERATIONS(string);

    static constexpr int npos = -1;

private:
    bool reallocate_if_needed(uint32_t size); // returns true when data was reallocted

    char* m_ptr;
    uint32_t m_size_reserved{ 0 };
};

template<typename It>
concept StringIt = requires() {
    is_same<It, string::Iterator>::value || is_same<It, string::ReversedIterator>::value
        || is_same<It, string::ConstIterator>::value || is_same<It, string::ConstReversedIterator>::value;
};

class string_view {
public:
    using value_type = char;
    using Type = char;
    using Iterator = string::Iterator;
    using ReversedIterator = string::ConstReversedIterator;
    using ConstIterator = string::ConstIterator;
    using ConstReversedIterator = string::ConstReversedIterator;

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

    ConstIterator begin(void) { return ConstIterator(m_ptr); }
    ConstIterator end(void) { return ConstIterator(m_ptr + m_size); }
    ConstReversedIterator rbegin(void) { return ConstReversedIterator(m_ptr + m_size - 1); }
    ConstReversedIterator rend(void) { return ConstReversedIterator(m_ptr - 1); }

    ConstIterator cbegin(void) { return ConstIterator(m_ptr); }
    ConstIterator cend(void) { return ConstIterator(m_ptr + m_size); }
    ConstReversedIterator crbegin(void) { return ConstReversedIterator(m_ptr + m_size - 1); }
    ConstReversedIterator crend(void) { return ConstReversedIterator(m_ptr - 1); }

private:
    char const* m_ptr;
    size_t m_size;
};

// ITERATOR CONSTRUCTOR
template<typename It>
    requires is_same<It, string::Iterator>::value || is_same<It, string::ConstIterator>::value
string::string(It beg, It end) : string()
{
    if (beg < end)
    {
        auto size_to_allocate = (uint32_t)(end.data() - beg.data());
        reallocate_if_needed(size_to_allocate);
        strncpy(m_ptr, beg.data(), size_to_allocate);
    }
}

// RITERATOR CONSTRUCTOR
template<typename It>
    requires is_same<It, string::ReversedIterator>::value || is_same<It, string::ConstReversedIterator>::value
string::string(It beg, It end) : string()
{
    if (beg < end)
    {
        auto size_to_allocate = (uint32_t)(beg.data() - end.data());
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

DEFINE_ITERATORS_CONVERTION_CONSTRUCTORS(string, NStringIterator);

DEFINE_ITERATOR_PLUSPLUS_PREFIX_OPERATOR(string, NStringIterator,
    {
        m_ptr++;
        return *this;
    });

DEFINE_ITERATOR_PLUSPLUS_POSTFIX_OPERATOR(string, NStringIterator,
    {
        auto itmp = *this;
        ++(this->m_ptr); //++(*this);

        return itmp;
    });

DEFINE_ITERATOR_MINUSMINUS_PREFIX_OPERATOR(string, NStringIterator,
    {
        m_ptr--;
        return *this;
    });

DEFINE_ITERATOR_MINUSMINUS_POSTFIX_OPERATOR(string, NStringIterator,
    {
        auto itmp = *this;
        --(this->m_ptr);

        return itmp;
    });

DEFINE_ITERATOR_PLUS_OPERATOR(string, NStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr + offset;
        return itmp;
    });

DEFINE_ITERATOR_MINUS_OPERATOR(string, NStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr - offset;
        return itmp;
    });

DEFINE_ITERATOR_ASTERISK_OPERATOR(string, NStringIterator,
    {
        return *m_ptr;
    });

DEFINE_ITERATOR_SPACESHIP_OPERATOR(string, NStringIterator,
    {
        return m_ptr == other.m_ptr ? 0 : m_ptr > other.m_ptr ? 1
                                                              : -1;
    });

DEFINE_ITERATOR_EQUALITY_OPERATOR(string, NStringIterator,
    {
        return m_ptr == other.m_ptr;
    });

DEFINE_ITERATOR_INEQUALITY_OPERATOR(string, NStringIterator,
    {
        return !(*this == other);
    });

DEFINE_ITERATOR_PLUSPLUS_PREFIX_OPERATOR(string, ReversedNStringIterator,
    {
        m_ptr--;
        return *this;
    });

DEFINE_ITERATOR_PLUSPLUS_POSTFIX_OPERATOR(string, ReversedNStringIterator,
    {
        auto itmp = *this;
        --m_ptr; //++(*this);

        return itmp;
    });

DEFINE_ITERATOR_MINUSMINUS_PREFIX_OPERATOR(string, ReversedNStringIterator,
    {
        m_ptr++;
        return *this;
    });

DEFINE_ITERATOR_MINUSMINUS_POSTFIX_OPERATOR(string, ReversedNStringIterator,
    {
        auto itmp = *this;
        ++m_ptr;

        return itmp;
    });

DEFINE_ITERATOR_PLUS_OPERATOR(string, ReversedNStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr - offset;
        return itmp;
    });

DEFINE_ITERATOR_MINUS_OPERATOR(string, ReversedNStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr + offset;
        return itmp;
    });

DEFINE_ITERATOR_ASTERISK_OPERATOR(string, ReversedNStringIterator,
    {
        return *m_ptr;
    });

DEFINE_ITERATOR_SPACESHIP_OPERATOR(string, ReversedNStringIterator,
    {
        return m_ptr == other.m_ptr ? 0 : m_ptr > other.m_ptr ? -1
                                                              : 1;
    });

DEFINE_ITERATOR_EQUALITY_OPERATOR(string, ReversedNStringIterator,
    {
        return m_ptr == other.m_ptr;
    });

DEFINE_ITERATOR_INEQUALITY_OPERATOR(string, ReversedNStringIterator,
    {
        return !(*this == other);
    });

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
