
#include "./string.h"
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/new.hpp>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/memory.hpp>
#include <sys/devices/com/com.h>

//KERNEL ALLOCATION

namespace std {

string::string(void) {
    m_ptr = (char*)calloc(1);
}

string::string(uint32_t size) : m_size_reserved(size + 1), m_ptr((char*)calloc(size + 2)) {}

string::string(char const* other)
{
    auto otherlen = strlen(other) + 1;
    m_ptr = (char*)calloc(otherlen);
    m_size_reserved = otherlen;
    strcpy(m_ptr, other);
}

string::string(char const* other, uint32_t size) : m_size_reserved(size + 1)
{
    m_ptr = (char*)calloc(m_size_reserved);
    strncpy(m_ptr, other, size);
}

string::string(string const& other)
    : m_size_reserved(other.m_size_reserved)
{
    m_ptr = (char*)calloc(m_size_reserved);
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
        // dbg_success("string", "omg i am used");
        free(m_ptr);
    }
}

int string::index_serialize(int index) const
{
    return index < 0 ? (int)length() + index : index;
}

uint32_t string::capacity(void) const
{
    return m_size_reserved > 0 ? m_size_reserved - sizeof('\0') : 0; // null char
}

void string::reserve(uint32_t size)
{
    size += 1;
    if (size > m_size_reserved) {
        m_size_reserved = size;
        m_ptr = (char*)realloc(m_ptr, m_size_reserved);
    }
}

void string::resize(uint32_t size, char c)
{
    auto str_len = length();
    reallocate_if_needed(size);

    if (size > str_len) {
        memset(end().data(), c, size - str_len);
        m_ptr[size] = '\0';
    }

    else {
        m_ptr[size] = '\0';
    }
}

uint32_t string::length(void) const
{
    if (m_size_reserved == 0) return 0;
    return strlen(m_ptr);
}

uint32_t string::size(void) const
{
    return length();
}

void string::clear(void)
{
    if (m_ptr != nullptr) {
        free(m_ptr);
    }

    m_size_reserved = 0;
    m_ptr = (char*)calloc(1);
}

string string::substr(int start_index, size_t len) const
{
    if (length() == 0) return "";
    auto begin_it = cbegin() + index_serialize(start_index);

    return string(begin_it, len == npos ? cend() :
        (begin_it + len >= cend() ? cend() : begin_it + len));
}

int string::last_of(const string& to_find, int start_index) const
{
    auto to_find_length = to_find.length();
    start_index = index_serialize(start_index) - to_find_length + 1;
    auto start = cbegin() + start_index;

    if (start < cbegin() || start >= cend()) return npos;

    int i = start_index;
    for (auto it = start; i != -1; it--, i--)
    {
        if (string(it, it + to_find_length) == to_find) {
            return i;
        }
    }

    return npos;
}

int string::first_of(const string& to_find, int start_index) const
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

string::operator bool(void) const
{
    return m_size_reserved != 0;
}

char& string::operator[](int index)
{
    return m_ptr[index_serialize(index)];
}

const char& string::operator[](int index) const
{
    return m_ptr[index_serialize(index)];
}

string& string::operator=(string const& other)
{
    reallocate_if_needed(other.m_size_reserved);
    strcpy(m_ptr, other.m_ptr);
    return *this;
}

string& string::operator=(string&& other)
{
    free(m_ptr);

    m_ptr = other.m_ptr;
    m_size_reserved = other.m_size_reserved;

    other.m_ptr = nullptr;
    other.m_size_reserved = 0;

    return *this;
}

string string::operator+(char c) const
{
    auto tstr = string(*this);
    tstr.reallocate_if_needed(tstr.m_size_reserved + sizeof(char));

    [&](uint32_t str_len) {
        tstr.m_ptr[str_len] = c;
        tstr.m_ptr[str_len + 1] = '\0';
    }(tstr.length());

    return tstr;
}

string string::operator+(const std::string& other) const
{
    auto tstr = string(length() + other.length());

    memcpy(tstr.m_ptr, m_ptr, length());
    memcpy(&tstr.m_ptr[tstr.length()], other.m_ptr, other.length());

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

string::Iterator string::begin()
{
    return m_ptr;
}

string::Iterator string::end()
{
    return m_ptr + length();
}

string::ConstIterator string::cbegin() const
{
    return m_ptr;
}

string::ConstIterator string::cend() const
{
    return m_ptr + length();
}

string::ReversedIterator string::rbegin()
{
    return m_ptr + length() - 1;
}

string::ReversedIterator string::rend()
{
    return m_ptr - 1;
}

string::ConstReversedIterator string::crbegin() const
{
    return m_ptr + length() - 1;
}

string::ConstReversedIterator string::crend() const
{
    return m_ptr - 1;
}

bool string::reallocate_if_needed(uint32_t size)
{
    size += 1;
    if (size > m_size_reserved)
    {
        m_size_reserved = size;
        m_ptr = (char*)realloc(m_ptr, size);
        return true;
    }

    return false;
}

///////////////////////ITERATORS////////////////////////////

} // namespace