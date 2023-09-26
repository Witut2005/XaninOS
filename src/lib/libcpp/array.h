
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

    private:
    Type* i_ptr;

    public: 

    ForwardArrayIterator(Type* ptr) : ForwardIterator<Arr>(ptr){this->i_ptr = ptr;}

    ForwardIterator<Arr>& operator ++ (void) override //prefix operator
    {
        asm("int 0");
        i_ptr++;
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
        i_ptr--;
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
        return *i_ptr;
    }

    bool operator == (const ForwardArrayIterator<Arr>& x)
    {
        return i_ptr == x.i_ptr;
    }

    bool operator != (const ForwardArrayIterator<Arr>& x)
    {
        return i_ptr != x.i_ptr;
    }

    operator bool(void)
    {
        return i_ptr != NULL;
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

    array(){}
    array(const array& arr) = default;
    array(std::initializer_list<T> a);
    ForwardArrayIterator<array<T, SIZE>> begin();
    ForwardArrayIterator<array<T, SIZE>> end();
    T& operator[](int32_t index);
    int find(T key);
    int find_other_than(T key);

};

template <class T, int SIZE>
array<T, SIZE>::array(std::initializer_list<T> a)
{
    auto it = a.begin();

    for(int i = 0; i < SIZE;i++) 
        arr[i] = it[i];
}

template <class T, int SIZE>
ForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::begin()
{
    return &arr[0];
}

template <class T, int SIZE>
ForwardArrayIterator<array<T, SIZE>> array<T, SIZE>::end()
{
    return &arr[SIZE];
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
