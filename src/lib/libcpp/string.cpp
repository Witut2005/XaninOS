
#include "./string.h"
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/memory.hpp>
#include <sys/devices/com/com.h>
#include <lib/libcpp/algorithm.h>

//KERNEL ALLOCATION

namespace std {

string::string(void)
{
    // dbg_info("string void", "");
    m_ptr = (char*)calloc(1);
    m_size_reserved = 1;
}

string::string(uint32_t size)
{
    m_size_reserved = size + 1;
    m_ptr = (char*)calloc(size + 1);
}

string::string(char const* other)
{
    auto otherlen = strlen(other) + 1;
    m_ptr = (char*)calloc(otherlen);
    m_size_reserved = otherlen;
    strcpy(m_ptr, other);
}

string::string(char const* other, uint32_t size)
{
    m_size_reserved = size + 1;
    m_ptr = (char*)calloc(size + 1);
    strncpy(m_ptr, other, size);
}

string::string(string const& other)
{
    m_size_reserved = other.m_size_reserved;
    m_ptr = (char*)calloc(other.m_size_reserved);
    strcpy(m_ptr, other.m_ptr);
}

string::string(string&& other)
{
    memcpy(this, &other, sizeof(string));
    other.raw_clear();
}

string::~string()
{
    free(m_ptr);
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

const string& string::resize(uint32_t size, char c)
{
    auto str_len = length();

    if (size > str_len)
    {
        m_size_reserved = reallocate_if_needed(size);
        memset(end().data(), c, size - str_len);
        m_ptr[size] = '\0';
    }

    else {
        m_ptr[size] = '\0';
    }
    return *this;
}

uint32_t string::length(void) const
{
    return strlen(m_ptr);
}

uint32_t string::size(void) const
{
    return length();
}

void string::raw_clear(void)
{
    m_size_reserved = 1;
    m_ptr = (char*)calloc(1);
}

void string::clear(void)
{
    free(m_ptr);

    m_size_reserved = 1;
    m_ptr = (char*)calloc(1);
}

void string::push_back(char c)
{
    reallocate_if_needed(length() + sizeof(char));
    *(uint16_t*)(end()).data() = c;
}

char string::pop_back(void)
{
    return std::get_and_set(*rbegin(), '\0');
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
    // dbg_info("copy operator =", "");
    reallocate_if_needed(other.m_size_reserved);
    strcpy(m_ptr, other.m_ptr);
    return *this;
}

string& string::operator=(string&& other)
{
    free(m_ptr);
    memcpy(this, &other, sizeof(string));

    other.raw_clear();
    return *this;
}

string string::operator+(char c) const
{
    auto tstr = string(*this);
    tstr.m_size_reserved = tstr.reallocate_if_needed(tstr.m_size_reserved + sizeof(char));

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

uint32_t string::reallocate_if_needed(uint32_t size)
{
    size += 1;

    if (size > m_size_reserved) {
        m_ptr = (char*)realloc(m_ptr, size * 2);
        return size * 2;
    }

    else {
        return m_size_reserved;
    }
}

///////////////////////ITERATORS////////////////////////////

} // namespace