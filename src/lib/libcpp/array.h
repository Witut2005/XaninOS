
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/initializer_list.hpp>
#include "./iterator.hpp"

namespace std
{

template<class Arr>
class ForwardArrayIterator : std::ForwardIterator<Arr>
{

    public: 
    using Type = typename Arr::Type;

    ForwardArrayIterator<Arr>(Type* ptr) : ForwardIterator<Arr>(ptr){}
    ForwardArrayIterator<Arr>(const ForwardArrayIterator<Arr>& other) : ForwardIterator<Arr>(other){}

    ForwardIterator<Arr>& operator ++ (void) override //prefix operator
    {
        this->i_ptr++;
        return *this;
    }

    ForwardIterator<Arr> operator ++ (int) override //postfix operator
    {
        ForwardArrayIterator tmp = *this;
        ++(this->i_ptr); //++(*this);

        return tmp;
    }

    ForwardIterator<Arr>& operator -- (void) override //prefix operator
    {
        this->i_ptr--;
        return *this;
    }

    ForwardIterator<Arr> operator -- (int) override //postfix operator
    {
        ForwardArrayIterator tmp = *this;
        --(this->i_ptr);

        return tmp;
    }

    Type& operator* (void)
    {
        return *this->i_ptr;
    }

    bool operator == (const ForwardArrayIterator<Arr>& x)
    {
        return this->i_ptr == x.i_ptr;
    }

    bool operator != (const ForwardArrayIterator<Arr>& x)
    {
        return this->i_ptr != x.i_ptr;
    }

    operator bool(void)
    {
        return this->i_ptr != NULL;
    }

};

template<class Arr>
class ReversedArrayIterator : std::ReversedIterator<Arr>
{

    public: 
    using Type = typename Arr::Type;

    ReversedArrayIterator(Type* ptr) : ReversedIterator<Arr>(ptr){}
    ReversedArrayIterator(const ReversedArrayIterator<Arr>& other) : ReversedIterator<Arr>(other){}

    ReversedIterator<Arr>& operator ++ (void) override //prefix operator
    {
        this->i_ptr--;
        return *this;
    }

    ReversedIterator<Arr> operator ++ (int) override //postfix operator
    {
        ReversedArrayIterator tmp = *this;
        --(this->i_ptr); //++(*this);

        return tmp;
    }

    ReversedIterator<Arr>& operator -- (void) override //prefix operator
    {
        this->i_ptr++;
        return *this;
    }

    ReversedIterator<Arr> operator -- (int) override //postfix operator
    {
        ReversedArrayIterator<Arr> tmp = *this;
        ++(this->i_ptr);

        return tmp;
    }

    Type& operator* (void)
    {
        return *this->i_ptr;
    }

    bool operator == (const ReversedArrayIterator<Arr>& x)
    {
        return this->i_ptr == x.i_ptr;
    }

    bool operator != (const ReversedArrayIterator<Arr>& x)
    {
        return this->i_ptr != x.i_ptr;
    }

    operator bool(void)
    {
        return this->i_ptr != NULL;
    }

};


template <class T, int SIZE>
class array
{

    private:
    T arr[SIZE];

    public:

    using Type = T;
    using iterator = ForwardArrayIterator< array<T, SIZE> >;
    constexpr static int ARR_SIZE = SIZE;

    array() = default;
    array(const array& arr) = default;
    array(std::initializer_list<T> a);
    constexpr ForwardArrayIterator<array<T, SIZE>> begin();
    constexpr ForwardArrayIterator<array<T, SIZE>> end();

    constexpr ForwardArrayIterator<array<T, SIZE>>* begin_ptr();
    constexpr ForwardArrayIterator<array<T, SIZE>>* end_ptr();

    constexpr ReversedArrayIterator<array<T, SIZE>> rbegin();
    constexpr ReversedArrayIterator<array<T, SIZE>> rend();
    T& operator[](int32_t index);
    int find(T key);
    int find_other_than(T key);

    template<class Cont, int TO_SIZE>
    std::array<T, TO_SIZE> slice(ForwardIterator<Cont>* begin)
    {
        std::array<T, TO_SIZE> tmp;

        for(int i = 0; i < TO_SIZE; i++, begin++)
            tmp[i] = *begin;

        return tmp;
    }

    // template<class Cont, int TO_SIZE>
    // std::array<T, TO_SIZE> slice(ReversedIterator<Cont>* rbegin)
    // {
    //     std::array<T, TO_SIZE> tmp;

    //     for(int i = 0; i < TO_SIZE; i++, rbegin++)
    //         tmp[i] = *rbegin;

    //     return tmp;
    // }

};

template <class T, int SIZE>
array<T, SIZE>::array(std::initializer_list<T> a)
{
    auto it = a.begin();

    for(int i = 0; i < SIZE;i++) 
        this->arr[i] = it[i];
}

template <class T, int SIZE>
constexpr ForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::begin()
{
    return &this->arr[0];
}

template <class T, int SIZE>
constexpr ForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::end()
{
    return &this->arr[SIZE];
}

template <class T, int SIZE>
constexpr ForwardArrayIterator<array<T, SIZE>>* array<T, SIZE>::begin_ptr()
{
    return new &this->arr[0];
}

template <class T, int SIZE>
constexpr ForwardArrayIterator<array<T, SIZE>>* array<T, SIZE>::end_ptr()
{
    return new &this->arr[SIZE];
}

template <class T, int SIZE>
constexpr ReversedArrayIterator<array<T, SIZE>> array<T, SIZE>::rbegin()
{
    return &arr[SIZE - 1];
}

template <class T, int SIZE>
constexpr ReversedArrayIterator<array<T, SIZE>> array<T, SIZE>::rend()
{
    return &arr[0] - 1;
}

template <class T, int SIZE>
T& array<T, SIZE>::operator[](int32_t index)
{
    if(index < 0)
        return arr[SIZE + index];
    return arr[index];
}

template <class T, int SIZE>
int array<T, SIZE>::find(T key)
{
    for(int i = 0; i < SIZE; i++)
    {
        if(arr[i] == key) 
            return i;
    }
    return -1;
}

template <class T, int SIZE>
int array<T, SIZE>::find_other_than(T key)
{
    for(int i = 0; i < SIZE; i++)
    {
        if(arr[i] != key) 
            return i;
    }
    return -1;
}



}
