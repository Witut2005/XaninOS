
#pragma once

#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>

namespace std
{

class ReversedStringIterator
{
private:
    char* i_ptr;

public:
    ReversedStringIterator(char* ptr) : i_ptr(ptr) {}
    ReversedStringIterator(const ReversedStringIterator& other) = default;

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

    bool operator==(const ReversedStringIterator x)
    {
        return i_ptr == x.i_ptr;
    }

    bool operator!=(const ReversedStringIterator x)
    {
        return i_ptr != x.i_ptr;
    }
};

class StringIterator
{
private:
    char* i_ptr;

public:
    StringIterator(char* ptr) : i_ptr(ptr) {}
    StringIterator(const StringIterator& other) = default;

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

    bool operator==(const StringIterator x)
    {
        return i_ptr == x.i_ptr;
    }

    bool operator!=(const StringIterator x)
    {
        return i_ptr != x.i_ptr;
    }
};

class string
{

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
    string(const char* str);
    string(const string& str);
    string(string&& str);
    inline ~string() { free(string_data); }

    string operator=(const char* x);
    string operator=(std::string x);
    std::string operator+(char character);
    std::string operator+(std::string second);
    std::string operator+(const char* second);

    inline bool operator==(std::string x)
    {
        return bstrcmp(this->c_str(), x.c_str());
    }

    inline bool operator!=(std::string x)
    {
        return !bstrcmp(this->c_str(), x.c_str());
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

}
