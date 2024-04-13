
#include "./string.h"
#include <lib/libcpp/new.hpp>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/memory.hpp>
#include <sys/devices/com/com.h>

//KERNEL ALLOCATION

namespace std {
string::string(uint32_t size) : m_size_reserved(size + 1), m_ptr(new char[size]) {}

string::string(char const* other)
{
    auto otherlen = strlen(other) + 1;
    m_ptr = new char[otherlen];
    m_size_reserved = otherlen;
    strcpy(m_ptr, other);
}

string::string(char const* other, uint32_t size) : m_size_reserved(size + 1)
{
    m_ptr = new char[m_size_reserved];
    strncpy(m_ptr, other, size);
}

string::string(string const& other)
    : m_size_reserved(other.m_size_reserved)
{
    m_ptr = new char[other.m_size_reserved];
    strcpy(m_ptr, other.m_ptr);
}

string::string(string&& other)
    : m_size_reserved(other.m_size_reserved)
    , m_ptr(other.m_ptr)
{
    other.m_size_reserved = 0;
    other.m_ptr = nullptr;
}

string::~string()
{
    if (m_ptr != nullptr) {
        free(m_ptr);
    }
}

int string::index_serialize(int index) const
{
    return index < 0 ? (int)length() + index : index;
}

uint32_t string::capacity(void) const
{
    return m_size_reserved > 0 ? m_size_reserved - SIZE_OF('\0') : 0; // null char
}

void string::reserve(uint32_t size)
{
    m_size_reserved = size + 1;
}

char const* string::c_str(void) const
{
    return m_ptr;
}

uint32_t string::length(void) const
{
    return strlen(m_ptr);
}

uint32_t string::size(void) const
{
    return length();
}

int string::last_of(std::string to_find, int start_index) const
{
    auto to_find_length = to_find.length();
    auto start = cbegin() + index_serialize(start_index);

    if (start < cbegin() || start >= cend()) return npos;
    if (start + to_find_length >= cend()) return npos;

    int i = length() - 1;
    for (auto it = start; it != cbegin() - 1; it--, i--)
    {
        if (string(it, it + to_find_length) == to_find) {
            return i;
        }
    }

    return npos;
}

int string::first_of(std::string to_find, int start_index) const
{
    auto to_find_length = to_find.length();
    auto start = cbegin() + index_serialize(start_index);

    if (start < cbegin() || start >= cend()) return npos;
    if (start + to_find_length >= cend()) return npos;

    int i = 0;
    for (auto it = start; it != cend(); it++, i++)
    {
        if (string(it, it + to_find_length) == to_find) {
            return i;
        }
    }

    return npos;
}

char& string::operator[](int index)
{
    return m_ptr[index_serialize(index)];
}

const char& string::operator[](int index) const
{
    return m_ptr[index_serialize(index)];
}

string& string::operator=(std::string const& other)
{
    if (m_ptr != nullptr) {
        free(m_ptr);
    }

    m_size_reserved = other.m_size_reserved;
    m_ptr = new char[other.m_size_reserved];

    strcpy(m_ptr, other.m_ptr);
    return *this;
}

string& string::operator=(std::string&& other)
{
    m_ptr = other.m_ptr;
    m_size_reserved = other.m_size_reserved;

    other.m_ptr = nullptr;
    other.m_size_reserved = 0;

    return *this;
}

string string::operator+(const std::string& other) const
{
    // reallocate_if_needed(length() + other.length());
    auto tstr = string(length() + other.length() + SIZE_OF('\0'));

    strcat(STRCAT_DEST_FIRST, tstr.m_ptr, m_ptr);
    strcat(STRCAT_DEST_FIRST, &tstr.m_ptr[tstr.length()], other.m_ptr);

    return tstr;
}

bool string::operator == (string const& other) const
{
    return bstrcmp(m_ptr, other.m_ptr);
}

bool string::operator != (string const& other) const
{
    return !(*this == other);
}

NStringIterator string::begin()
{
    return m_ptr;
}

NStringIterator string::end()
{
    return m_ptr + length();
}

ConstNStringIterator string::cbegin() const
{
    return m_ptr;
}

ConstNStringIterator string::cend() const
{
    return m_ptr + length();
}

ReversedNStringIterator string::rbegin()
{
    return m_ptr + length() - 1;
}

ReversedNStringIterator string::rend()
{
    return m_ptr - 1;
}

ConstReversedNStringIterator string::crbegin() const
{
    return m_ptr + length() - 1;
}

ConstReversedNStringIterator string::crend() const
{
    return m_ptr - 1;
}


// uint32_t string::reserved_space_size_get(void) const {
//     return size_to_mmngr_blocks(m_size_reserved) * XANIN_PMMNGR_BLOCK_SIZE;
// }

bool string::reallocate_if_needed(uint32_t size)
{
    size += 1;
    if (size > m_size_reserved)
    {
        m_size_reserved = size;
        if (m_ptr == nullptr) {
            m_ptr = (char*)calloc(size);
        }
        else {
            m_ptr = (char*)realloc(m_ptr, size);
        }
        return true;
    }

    return false;
}

DEFINE_ITERATORS_CONVERTION_CONSTRUCTORS(NStringIterator)

DEFINE_ITERATOR_PLUSPLUS_PREFIX_OPERATOR(NStringIterator,
    {
        m_ptr++;
        return *this;
    }
);

DEFINE_ITERATOR_PLUSPLUS_POSTFIX_OPERATOR(NStringIterator,
    {
        auto itmp = *this;
        ++(this->m_ptr); //++(*this);

        return itmp;
    }
);

DEFINE_ITERATOR_MINUSMINUS_PREFIX_OPERATOR(NStringIterator,
    {
        m_ptr--;
        return *this;
    }
);

DEFINE_ITERATOR_MINUSMINUS_POSTFIX_OPERATOR(NStringIterator,
    {
        auto itmp = *this;
        --(this->m_ptr);

        return itmp;
    }
);

DEFINE_ITERATOR_PLUS_OPERATOR(NStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr + offset;
        return itmp;
    }
);

DEFINE_ITERATOR_MINUS_OPERATOR(NStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr - offset;
        return itmp;
    }
);

DEFINE_ITERATOR_ASTERISK_OPERATOR(NStringIterator, char&,
    {
        return *m_ptr;
    }
);

DEFINE_ITERATOR_SPACESHIP_OPERATOR(NStringIterator,
    {
        return m_ptr == other.m_ptr ? 0 : m_ptr > other.m_ptr ? 1 : -1;
    }
);

DEFINE_ITERATOR_EQUALITY_OPERATOR(NStringIterator,
    {
        return m_ptr == other.m_ptr;
    }
);

DEFINE_ITERATOR_INEQUALITY_OPERATOR(NStringIterator,
    {
        return !(*this == other);
    }
);

DEFINE_ITERATOR_PLUSPLUS_PREFIX_OPERATOR(ReversedNStringIterator,
    {
        m_ptr--;
        return *this;
    }
);

DEFINE_ITERATOR_PLUSPLUS_POSTFIX_OPERATOR(ReversedNStringIterator,
    {
        auto itmp = *this;
        --m_ptr; //++(*this);

        return itmp;
    }
);

DEFINE_ITERATOR_MINUSMINUS_PREFIX_OPERATOR(ReversedNStringIterator,
    {
        m_ptr++;
        return *this;
    }
);

DEFINE_ITERATOR_MINUSMINUS_POSTFIX_OPERATOR(ReversedNStringIterator,
    {
        auto itmp = *this;
        ++m_ptr;

        return itmp;
    }
);

// DEFINE_ITERATOR_FUNCTIONALITY(ReversedNStringIterator, +, ReversedNStringIterator, int offset,
DEFINE_ITERATOR_PLUS_OPERATOR(ReversedNStringIterator, int offset,
    {
        auto itmp = *this;
        itmp.m_ptr = itmp.m_ptr - offset;
        return itmp;
    }
);

DEFINE_ITERATOR_MINUS_OPERATOR(ReversedNStringIterator, int offset,
    {
    auto itmp = *this;
    itmp.m_ptr = itmp.m_ptr + offset;
    return itmp;
    }
);

DEFINE_ITERATOR_ASTERISK_OPERATOR(ReversedNStringIterator, char&,
    {
        return *m_ptr;
    }
);

DEFINE_ITERATOR_SPACESHIP_OPERATOR(ReversedNStringIterator,
    {
        return m_ptr == other.m_ptr ? 0 : m_ptr > other.m_ptr ? -1 : 1;
    }
);

DEFINE_ITERATOR_EQUALITY_OPERATOR(ReversedNStringIterator,
    {
    return m_ptr == other.m_ptr;
    }
);

DEFINE_ITERATOR_INEQUALITY_OPERATOR(ReversedNStringIterator,
    {
    return !(*this == other);
    }
);

///////////////////////ITERATORS////////////////////////////

} // namespace