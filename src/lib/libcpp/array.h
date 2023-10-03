
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/container.hpp>
#include "./iterator.hpp"

namespace std
{

template<class Arr>
class ForwardArrayIterator : public std::ForwardIterator<Arr>
{

    public: 
    using value_type = typename Arr::value_type;

    using lreference = typename Arr::lreference;
    using rreference = typename Arr::rreference;

    using const_lreference = typename Arr::const_lreference;
    using const_rreference = typename Arr::const_rreference;

    ForwardArrayIterator<Arr>(value_type* ptr){this->i_ptr = ptr;}
    ForwardArrayIterator<Arr>(const ForwardIterator<Arr>& other) {this->i_ptr = other.i_ptr;}
    ForwardArrayIterator<Arr>(const ForwardArrayIterator<Arr>& other) {this->i_ptr = other.i_ptr;}
    // ForwardArrayIterator<Arr>(ForwardIterator<Arr>&& other) 

    ForwardIterator<Arr>& operator ++ (void) override //prefix operator
    {
        this->i_ptr++;
        return *this;
    }

    ForwardIterator<Arr>&& operator ++ (int) override //postfix operator
    {
        ++(this->i_ptr); 
        ForwardArrayIterator tmp = *this;

        return std::move(tmp);
    }

    ForwardIterator<Arr>& operator -- (void) override //prefix operator
    {
        this->i_ptr--;
        return *this;
    }

    ForwardIterator<Arr>&& operator -- (int) override //postfix operator
    {
        --(this->i_ptr);
        ForwardArrayIterator tmp = *this;

        return std::move(tmp);
    }

    ForwardIterator<Arr>&& operator + (int offset) override 
    {
        ForwardArrayIterator tmp = *this;

        for(int i = 0; i < offset; i++)
            tmp.i_ptr++;

        return std::move(tmp);
    }

    ForwardIterator<Arr>&& operator - (int offset) override 
    {
        ForwardArrayIterator tmp = *this;
        
        for(int i = 0; i < offset; i++)
            tmp.i_ptr--;

        return std::move(tmp);
    }

    lreference operator* (void) override
    {
        return *this->i_ptr;
    }

    ForwardIterator<Arr>& operator = (ForwardIterator<Arr>&& other) override 
    {
        this->i_ptr = other.i_ptr;
    }

    bool operator == (const ForwardIterator<Arr>& x) override
    {
        return this->i_ptr == x.i_ptr;
    }

    bool operator != (const ForwardIterator<Arr>& x) override
    {
        return this->i_ptr != x.i_ptr;
    }

    operator bool(void) override
    {
        return this->i_ptr != NULL;
    }

};

template<class Arr>
class ReversedArrayIterator : public std::ReversedIterator<Arr>
{

    public: 
    using value_type = typename Arr::value_type;

    using lreference = typename Arr::lreference;
    using rreference = typename Arr::rreference;

    using const_lreference = typename Arr::const_lreference;
    using const_rreference = typename Arr::const_rreference;

    ReversedArrayIterator(value_type* ptr) : ReversedIterator<Arr>(ptr){}
    ReversedArrayIterator(const ReversedArrayIterator<Arr>& other) : ReversedIterator<Arr>(other){}

    ReversedIterator<Arr>& operator ++ (void) override //prefix operator
    {
        this->i_ptr--;
        return *this;
    }

    ReversedIterator<Arr>&& operator ++ (int) override //postfix operator
    {
        --(this->i_ptr); 
        ReversedArrayIterator tmp = *this;

        return std::move(tmp);
    }

    ReversedIterator<Arr>& operator -- (void) override //prefix operator
    {
        this->i_ptr++;
        return *this;
    }

    ReversedIterator<Arr>&& operator -- (int) override //postfix operator
    {
        ++(this->i_ptr);
        ReversedArrayIterator<Arr> tmp = *this;

        return std::move(tmp);
    }

    ReversedIterator<Arr>&& operator + (int offset) override 
    {
        ReversedArrayIterator tmp = *this;

        for(int i = 0; i < offset; i++)
            tmp.i_ptr++;

        return std::move(tmp);
    }

    ReversedIterator<Arr>&& operator - (int offset) override 
    {
        ReversedArrayIterator tmp = *this;
        
        for(int i = 0; i < offset; i++)
            tmp.i_ptr--;

        return std::move(tmp);
    }

    lreference operator* (void) override
    {
        return *this->i_ptr;
    }

    bool operator == (const ReversedIterator<Arr>& x) override
    {
        return this->i_ptr == x.i_ptr;
    }

    bool operator != (const ReversedIterator<Arr>& x) override
    {
        return this->i_ptr != x.i_ptr;
    }

    operator bool(void) override
    {
        return this->i_ptr != NULL;
    }

};


template <class T, int SIZE>
class array : Container<T>
{

    private:
    T arr[SIZE];

    public:

    using value_type = T;

    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;

    using forward_iterator = ForwardArrayIterator<array<T, SIZE>>;
    using reversed_iterator = ReversedArrayIterator<array<T, SIZE>>;

    array() = default;
    array(const array& arr) = default;
    array(std::initializer_list<T> a);

    constexpr forward_iterator begin();
    constexpr forward_iterator end();

    constexpr reversed_iterator rbegin();
    constexpr reversed_iterator rend();

    T* pointer_get(void) override;

    std::array<T, SIZE>& operator = (const std::array<T, SIZE>& other) = default;
    lreference operator[](int index);

    T get_copy(int32_t index) const;
    int find(T key);
    int find_other_than(T key);

    T& front(void) override;
    T& back(void) override;

    constexpr int size(void) override
    {
        return SIZE;
    }

    template<int OTHER_SIZE>
    auto concat(const std::array<T, OTHER_SIZE>& other) 
    {
        std::array<T, SIZE + OTHER_SIZE> tmp;

        for(int i = 0; i < SIZE; i++)
            tmp[i] = other.get_copy(i);

        for(int i = SIZE; i < SIZE + OTHER_SIZE; i++)
            tmp[i] = other.get_copy(i - SIZE);
        
        return tmp;
    }

    template<int TO_SIZE>
    std::array<T, TO_SIZE> slice(const ForwardIterator<std::array<T, SIZE>>& begin) 
    {
        std::array<T, TO_SIZE> tmp;
        ForwardArrayIterator<std::array<T, SIZE>> it(begin);

        for(int i = 0; i < TO_SIZE; i++, it++)
            tmp[i] = *it;

        return tmp;
    }


    template<int TO_SIZE>
    std::array<T, TO_SIZE> slice(ReversedIterator<std::array<T, SIZE>>&& rbegin)
    {
        std::array<T, TO_SIZE> tmp;

        for(int i = 0; i < TO_SIZE; i++, rbegin++)
            tmp[i] = *rbegin;

        return tmp;
    }

    // void print(void);
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
T& array<T, SIZE>::operator[](int index)
{
    if(index < 0)
        return arr[SIZE + index];
    return arr[index];
}

template <class T, int SIZE>
T array<T, SIZE>::get_copy(int32_t index) const
{
    return this->arr[index];
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

template<typename T, int SIZE>
T* array<T, SIZE>::pointer_get(void) 
{
    return this->arr;
}

template<typename T, int SIZE>
T& array<T, SIZE>::front(void) 
{
    return this->arr[0];
}

template<typename T, int SIZE>
T& array<T, SIZE>::back(void) 
{
    return this->arr[SIZE - 1];
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

//global scope functions
template<typename T, int SIZE>
std::array<T, SIZE> to_array(T* ptr)
{
    std::array<T, SIZE> arr;

    for(int i = 0; i < SIZE; i++)
        arr[i] = ptr[i];
}

}
