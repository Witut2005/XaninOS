
#include "./string.h"

namespace std {

string::string(StringIterator beg, StringIterator end)
{
    this->string_data = (char*)calloc(4096);
    int i = 0;
    for (auto it = beg; it != end; it++, i++)
        this->string_data[i] = *it;
}

string::string(ReversedStringIterator rbeg, ReversedStringIterator rend)
{
    this->string_data = (char*)calloc(4096);

    int i = 0;
    for (auto it = rbeg; it != rend; it++, i++)
        this->string_data[i] = *it;
}

string::string(char* str)
{
    this->string_data = (char*)calloc(SIZE_OF(char) * strlen(str) + 1);
    strcpy(this->string_data, str);
}

string::string(char const* str)
{
    this->string_data = (char*)calloc(SIZE_OF(char) * strlen(str) + 1);
    strcpy(this->string_data, str);
}

string::string(string const& str)
{
    this->string_data = (char*)calloc(SIZE_OF(char) * strlen((char*)str.c_str()) + 1);
    strcpy(this->string_data, str.c_str());
}

string::string(string&& str)
{
    this->string_data = str.string_data;
    str.string_data = NULL;
}

string string::operator=(char const* x)
{
    realloc(string_data, strlen(x));
    strcpy(string_data, x);
    return *this;
}

string string::operator=(std::string x)
{
    realloc(string_data, strlen(x.c_str()));
    strcpy(string_data, x.c_str());
    return *this;
}

string string::operator+(char character)
{
    this->string_data[strlen(this->string_data)] = character;
    return *this;
}

string string::operator+(std::string second)
{

    char* c = (char*)malloc(second.length() + this->length());

    int i = 0;
    for (; this->c_str()[i] != '\0'; i++)
        c[i] = this->c_str()[i];

    int j = 0;
    for (; second.c_str()[j] != '\0'; i++, j++)
        c[i] = second.c_str()[j];

    std::string result(c);
    return result;
}

string string::operator+(char const* second)
{
    char* c = (char*)malloc(strlen(second) + this->length());

    int i = 0;
    for (; this->c_str()[i] != '\0'; i++)
        c[i] = this->c_str()[i];

    int j = 0;
    for (; second[j] != '\0'; i++, j++)
        c[i] = second[j];

    std::string result(c);
    return result;
}

StringIterator string::begin() const
{
    return string_data;
}

ReversedStringIterator string::rbegin() const
{
    return string_data + strlen(string_data) - 1;
}

StringIterator string::end() const
{
    return string_data + strlen(string_data);
}

ReversedStringIterator string::rend() const
{
    return string_data - 1;
}

uint32_t string::size()
{
    char* tmp = this->c_str();
    uint32_t size = 0;

    while (*tmp != '\0')
    {
        size++;
        tmp++;
    }

    return size;
}

// nstring(StringIterator beg, StringIterator end)
// {
// }

// nstring(ReversedStringIterator rbeg, ReversedStringIterator rend)
// {
// }

nstring::nstring(char const* other)
    : m_ptr(new char[strlen(other)])
    , m_size_reserved(m_size_reserved)
{
}

nstring::nstring(nstring const& other)
    : m_size_reserved(other.m_size_reserved)
{
    m_ptr = new char[other.m_size_reserved];
    strcpy(m_ptr, other.m_ptr);
}

nstring::nstring(nstring&& other)
    : m_size_reserved(other.m_size_reserved)
    , m_ptr(other.m_ptr)
{
    m_size_reserved = 0;
    other = nullptr;
}

nstring::~nstring()
{
    free(m_ptr);
}

uint32_t nstring::size(void) const
{
    return m_size_reserved;
}

void nstring::reserve(uint32_t size)
{
    m_size_reserved = size;
}

char const* nstring::c_str(void) const
{
    return m_ptr;
}

uint32_t nstring::length(void) const
{
    return strlen(m_ptr);
}

nstring& nstring::operator=(std::nstring const& other)
{
    m_size_reserved = other.m_size_reserved;

    m_ptr = new char[other.m_size_reserved];
    strcpy(m_ptr, other.m_ptr);
}

nstring& nstring::operator+(char character)
{
}

nstring& nstring::operator+(std::nstring&& other)
{
}

// uint32_t nstring::reserved_space_size_get(void) const {
//     return size_to_mmngr_blocks(m_size_reserved) * XANIN_PMMNGR_BLOCK_SIZE;
// }

bool nstring::reallocate_if_needed(uint32_t size)
{
    if (size > m_size_reserved)
    {
        m_size_reserved = size;
        m_ptr = (char*)realloc(m_ptr, size);
        return true;
    }

    return false;
}

} // namespace
