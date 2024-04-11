
#pragma once

#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libcpp/container/iterator.hpp>
#include <lib/libcpp/type_traits.h>

namespace std {

BASIC_ITERATOR_DECLARE(NStringIterator, char)
public:
    NStringIterator operator+(int offset);
    NStringIterator operator-(int offset);
};

BASIC_CONSTANT_ITERATOR_DECLARE(ConstNStringIterator, char)
public:
    ConstNStringIterator operator+(int offset);
    ConstNStringIterator operator-(int offset);
};

BASIC_ITERATOR_DECLARE(ReversedNStringIterator, char)
public:
    ReversedNStringIterator operator+(int offset);
    ReversedNStringIterator operator-(int offset);
};

BASIC_CONSTANT_ITERATOR_DECLARE(ConstReversedNStringIterator, char)
public:
    ConstReversedNStringIterator operator+(int offset);
    ConstReversedNStringIterator operator-(int offset);
};

template <typename T>
concept StringIt = is_same<T, NStringIterator>::value || is_same<T, ReversedNStringIterator>::value
|| is_same<T, ConstNStringIterator>::value || is_same<T, ConstReversedNStringIterator>::value;

class string {
public:
    using Type = char;
    using Iterator = NStringIterator;
    using ReversedIterator = ReversedNStringIterator;
    using ConstIterator = ConstNStringIterator;
    using ConstReversedIterator = ConstReversedNStringIterator;

    string(void) = default;
    explicit string(uint32_t size);

    // template <typename It>
    //     requires StringIt<It>
    // string(It beg, It end);
    string(NStringIterator beg, NStringIterator end);

    string(char const* str);
    explicit string(char const* str, uint32_t size);
    string(string const& str);
    string(string&& str);
    ~string(void);

    void reserve(uint32_t size); //reserves to hold size characters
    char const* c_str(void) const;
    uint32_t capacity(void) const; //returns m_size_reserved - sizeof('\0')
    uint32_t length(void) const;
    uint32_t size(void) const;

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

// template <typename It>
//     requires StringIt<It>
// string::string(It beg, It end)
// {
//     for (int i = 1;beg != end; beg++, i++) {
//         reallocate_if_needed(i);
//         m_ptr[i - 1] = *beg;
//     }
// }

} // namespace

