
#pragma once

#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>

namespace std {

class ReversedStringIterator {
private:
    char* i_ptr;

public:
    ReversedStringIterator(char* ptr)
        : i_ptr(ptr)
    {
    }
    ReversedStringIterator(ReversedStringIterator const& other) = default;

    ReversedStringIterator& operator++() // prefix operator
    {
        i_ptr--;
        return *this;
    }

    ReversedStringIterator operator++(int) // postfix operator
    {
        ReversedStringIterator tmp = *this;
        --(this->i_ptr); //++(*this);

        return tmp;
    }

    ReversedStringIterator& operator--() // prefix operator
    {
        i_ptr++;
        return *this;
    }

    ReversedStringIterator operator--(int) // postfix operator
    {
        ReversedStringIterator tmp = *this;
        ++(this->i_ptr);

        return tmp;
    }

    char& operator*()
    {
        return *i_ptr;
    }

    bool operator==(ReversedStringIterator const x)
    {
        return i_ptr == x.i_ptr;
    }

    bool operator!=(ReversedStringIterator const x)
    {
        return i_ptr != x.i_ptr;
    }
};

class StringIterator {
private:
    char* i_ptr;

public:
    StringIterator(char* ptr)
        : i_ptr(ptr)
    {
    }
    StringIterator(StringIterator const& other) = default;

    StringIterator& operator++() // prefix operator
    {
        i_ptr++;
        return *this;
    }

    StringIterator operator++(int) // postfix operator
    {
        StringIterator tmp = *this;
        ++(this->i_ptr); //++(*this);

        return tmp;
    }

    StringIterator& operator--() // prefix operator
    {
        i_ptr--;
        return *this;
    }

    StringIterator operator--(int) // postfix operator
    {
        StringIterator tmp = *this;
        --(this->i_ptr);

        return tmp;
    }

    char& operator*()
    {
        return *i_ptr;
    }

    bool operator==(StringIterator const x)
    {
        return i_ptr == x.i_ptr;
    }

    bool operator!=(StringIterator const x)
    {
        return i_ptr != x.i_ptr;
    }
};

class string {

private:
    char* string_data;

public:
    using Type = char;
    using Iterator = StringIterator;

    inline string()
    {
        this->string_data = (char*)calloc(512);
    }
    string(StringIterator beg, StringIterator end);
    string(ReversedStringIterator rbeg, ReversedStringIterator rend);
    string(char* str);
    string(char const* str);
    string(string const& str);
    string(string&& str);
    inline ~string() { free(string_data); }

    string operator=(char const* x);
    string operator=(std::string x);
    std::string operator+(char character);
    std::string operator+(std::string second);
    std::string operator+(char const* second);

    inline bool operator==(std::string x)
    {
        return bstrcmp(this->c_str(), x.c_str());
    }

    inline bool operator!=(std::string x)
    {
        return !(*this == x);
    }

    inline char& operator[](uint32_t index)
    {
        return this->string_data[index];
    }

    StringIterator begin() const;
    ReversedStringIterator rbegin() const;
    StringIterator end() const;
    ReversedStringIterator rend() const;

    uint32_t size();

    inline void reserve(uint32_t size)
    {
        realloc(this->string_data, size * SIZE_OF(char));
    }

    inline char* c_str() const
    {
        return string_data;
    }

    inline uint32_t length() const
    {
        return this->string_data == NULL ? 0 : strlen(this->string_data);
    }
};

class nstring {
public:
    using Type = char;
    using Iterator = StringIterator;

    nstring(void) = default;
    nstring(StringIterator beg, StringIterator end);
    nstring(ReversedStringIterator rbeg, ReversedStringIterator rend);
    nstring(char const* str);
    nstring(nstring const& str);
    nstring(nstring&& str);
    ~nstring() { free(m_ptr); }

    uint32_t size(void) const;
    void reserve(uint32_t size);
    char const* c_str(void) const;
    uint32_t length(void) const;

    nstring& operator=(nstring const& other);
    nstring& operator+(char character);
    nstring& operator+(nstring&& other);

    char& operator[](uint32_t index);

    StringIterator begin() const;
    ReversedStringIterator rbegin() const;
    StringIterator end() const;
    ReversedStringIterator rend() const;

private:
    uint32_t reserved_space_size_get(void) const; // data are allocated in 512 byte blocks
    bool reallocate_if_needed(uint32_t size);     // returns true when data was reallocted

    char* m_ptr { nullptr };
    uint32_t m_size_reserved { 0 };
};

} // namespace
