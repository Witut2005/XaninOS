
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>
#include "./iterator.hpp"

namespace std
{

template <class T, int SIZE>
class array
{

private:
    T ptr[SIZE];

public:
    using this_type = array<T, SIZE>;

    using value_type = T;
    using iterable_type = T*;

    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;

    using forward_iterator = ForwardArrayIterator<this_type>;
    using reversed_iterator = ReversedArrayIterator<this_type>;

    using const_forward_iterator = ConstForwardArrayIterator<this_type>;
    using const_reversed_iterator = ConstReversedArrayIterator<this_type>;

    array();
    array(const array& arr) = default;
    array(std::initializer_list<T> a);

    template <typename InputIt>
    array(InputIt beg, InputIt end)
    {
        int i = 0;

        for (; (beg != end) & (i < SIZE); beg++, i++)
        {
            memcpy((uint8_t*)&this->ptr[i], (uint8_t*)beg.pointer(), sizeof(T));
        }
    }

    inline constexpr iterable_type
        begin_ptr()
    {
        return ptr;
    }
    inline constexpr iterable_type end_ptr()
    {
        return ptr + SIZE;
    }
    inline constexpr iterable_type rbegin_ptr()
    {
        return ptr + SIZE - 1;
    }
    inline constexpr iterable_type rend_ptr()
    {
        return ptr - 1;
    }

    inline constexpr const iterable_type cbegin_ptr()
    {
        return ptr;
    }
    inline constexpr const iterable_type cend_ptr()
    {
        return ptr + SIZE;
    }
    inline constexpr const iterable_type crbegin_ptr()
    {
        return ptr + SIZE - 1;
    }
    inline constexpr const iterable_type crend_ptr()
    {
        return ptr - 1;
    }

    constexpr forward_iterator begin();
    constexpr forward_iterator end();
    constexpr reversed_iterator rbegin();
    constexpr reversed_iterator rend();

    constexpr const_forward_iterator cbegin();
    constexpr const_forward_iterator cend();
    constexpr const_reversed_iterator crbegin();
    constexpr const_reversed_iterator crend();

    T* pointer(void); // override;

    std::array<T, SIZE>& operator=(const std::array<T, SIZE>& other) = default;
    constexpr lreference operator[](const int index);

    constexpr bool valid_element(T& element) const;

    T get_copy(int32_t index) const;
    int find_default_handler(T arg) const;
    int find_other_than_default_handler(T arg) const;
    // int find(T value, auto finder);

    int find_other_than(T value, auto finder);

    T& front(void); // override;
    T& back(void);  // override;

    constexpr int size(void) // override
    {
        return SIZE;
    }

    template <int OTHER_SIZE>
    auto concat(const std::array<T, OTHER_SIZE>& other)
    {
        std::array<T, SIZE + OTHER_SIZE> tmp;

        for (int i = 0; i < SIZE; i++)
            tmp[i] = other.get_copy(i);

        for (int i = SIZE; i < SIZE + OTHER_SIZE; i++)
            tmp[i] = other.get_copy(i - SIZE);

        return tmp;
    }

    template <int TO_SIZE, typename InputIt>
    std::array<T, TO_SIZE> slice(InputIt&& begin)
    {

        static_assert((begin.type == Types::ReversedArrayIterator) || (begin.type == Types::ForwardArrayIterator),
            "You need to use ArrayIterator object");

        std::array<T, TO_SIZE> tmp;

        for (int i = 0; i < TO_SIZE; i++, begin++)
        {
            if (!begin.valid())
                break;
            tmp[i] = *begin;
        }

        return tmp;
    }

    template <typename Arr>
    friend class ForwardArrayIterator;

    template <typename Arr>
    friend class ReversedArrayIterator;

    template <typename Arr>
    friend class ConstForwardArrayIterator;

    template <typename Arr>
    friend class ConstReversedArrayIterator;
    // void print(void);
};

template <class T, int SIZE>
array<T, SIZE>::array()
{
    memset((uint8_t*)this->ptr, 0, sizeof(T) * SIZE);
}

template <class T, int SIZE>
array<T, SIZE>::array(std::initializer_list<T> a)
{
    auto it = a.begin();

    for (int i = 0; i < SIZE; i++)
        memcpy((uint8_t*)this->ptr[i], &it[i], sizeof(T));
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::forward_iterator array<T, SIZE>::begin()
{
    return array<T, SIZE>::forward_iterator(&this->ptr[0], *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::forward_iterator array<T, SIZE>::end()
{
    return array<T, SIZE>::forward_iterator(&this->ptr[SIZE], *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::reversed_iterator array<T, SIZE>::rbegin()
{
    return array<T, SIZE>::reversed_iterator(&ptr[SIZE - 1], *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::reversed_iterator array<T, SIZE>::rend()
{
    return array<T, SIZE>::reversed_iterator(ptr - 1, *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::const_forward_iterator array<T, SIZE>::cbegin()
{
    return array<T, SIZE>::const_forward_iterator(&this->ptr[0], *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::const_forward_iterator array<T, SIZE>::cend()
{
    return array<T, SIZE>::const_forward_iterator(&this->ptr[SIZE], *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::const_reversed_iterator array<T, SIZE>::crbegin()
{
    return array<T, SIZE>::const_reversed_iterator(&ptr[SIZE - 1], *this);
}

template <class T, int SIZE>
constexpr typename array<T, SIZE>::const_reversed_iterator array<T, SIZE>::crend()
{
    return array<T, SIZE>::const_reversed_iterator(ptr - 1, *this);
}

template <class T, int SIZE>
constexpr T& array<T, SIZE>::operator[](const int index)
{

    if (index < 0)
    {
        if (index < SIZE * (-1))
            return *(T*)NULL;

        return this->ptr[SIZE + index];
    }

    else if (index > SIZE)
        return *(T*)NULL;

    return this->ptr[index];
}

template <class T, int SIZE>
constexpr bool array<T, SIZE>::valid_element(T& element) const
{
    return ((uint32_t)&element >= (uint32_t)this->ptr) & ((uint32_t)&element < (uint32_t)&this->ptr[SIZE]);
}

template <class T, int SIZE>
T array<T, SIZE>::get_copy(int32_t index) const
{
    return this->ptr[index];
}

template <class T, int SIZE>
int std::array<T, SIZE>::find_default_handler(T arg) const
{
    for (int i = 0; i < SIZE; i++)
    {
        if (this->ptr[i] == arg)
            return i;
    }
    return -1;
}

template <typename T, int SIZE>
T* array<T, SIZE>::pointer(void)
{
    return this->ptr;
}

template <typename T, int SIZE>
T& array<T, SIZE>::front(void)
{
    return this->ptr[0];
}

template <typename T, int SIZE>
T& array<T, SIZE>::back(void)
{
    return this->ptr[SIZE - 1];
}

// template<typename T, int SIZE>
// void array<T, SIZE>::print(void)
// {

//     if(!this->size())
//     {
//         std::cout << "[]" << std::endl;
//         return;
//     }

//     std::cout << "[";
//     auto it = this->begin();

//     for(; it != this->end()-2; it++)
//         std::cout << *it << ", ";

//     std::cout << *(it + 1);
//     std::cout << "]";
// }

// global scope functions
template <typename T, int SIZE>
std::array<T, SIZE> to_array(T* ptr)
{
    std::array<T, SIZE> arr;

    for (int i = 0; i < SIZE; i++)
        arr[i] = ptr[i];
}

template<typename T, int Size>
class narray {
    public:
    using value_type = T;
    using Iterator = NArrayIterator<T>;
    using ReversedIterator = ConstReversedNArrayIterator;
    using ConstIterator = ConstNArrayIterator;
    using ConstReversedIterator = ConstReversedNIterator;

    array(void) = default;

    // string(const NStringIterator beg, const NStringIterator end);
    template <typename It>
    array(It beg);

    int index_serialize(int index) const;
    constexpr const char * data(void) { return m_ptr; }
    constexpr const char * data(void) const {return m_ptr; }
    constexpr uint32_t size(void) const { return Size; }

    // int last_of(std::string to_find, int start_index = -1) const;
    // int first_of(std::string to_find, int start_index = 0) const;

    char& operator[](int index) { return m_ptr[index_serialize(index)]; }
    const char& operator[](int index) const { return m_ptr[index_serialize(index)]; }

    narray& operator=(narray const& other);
    bool operator == (narray const& other) const;
    bool operator != (narray const& other) const;

    DEFINE_CLASS_RANGE_OPERATIONS(NArrayIterator);

    static constexpr int npos = -1;

    private:
    T m_ptr[Size];
};

// RANDOM_ACCESS_ITERATORS_DECLARE(NArrayIterator, narray)

} // namespace