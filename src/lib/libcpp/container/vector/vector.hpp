
#pragma once

#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/algorithm.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/type_traits.h>

#include "./iterator.hpp"

namespace std {
    
template <typename T>
class vector
{
    
private:
    T* ptr;
    uint32_t v_size = 0;
    
public:
    using value_type = T;
    using iterable_type = T*;

    using lreference = T&;
    using rreference = T&&;

    using const_lreference = const T&;
    using const_rreference = const T&&;

    using forward_iterator = ForwardVectorIterator<vector<T>>;
    using reversed_iterator = ReversedVectorIterator<vector<T>>;

    using const_forward_iterator = ConstForwardVectorIterator<vector<T>>;
    using const_reversed_iterator = ConstReversedVectorIterator<vector<T>>;

    vector();
    vector(const vector<T>& other) =  default; // copy constructor
    vector(vector<T>&& other); // move constructor
    vector (std::initializer_list<T> items); 

    template<typename InputIt>
    vector(InputIt beg, InputIt end) : vector()
    {
        for(; beg != end; beg++) 
            this->push_back(*beg);
    }

    ~vector(); 

    std::vector<T>& operator = (const vector<T>& other) = default; // copy assigment operator
    std::vector<T>& operator = (vector<T>&& other); // move assigment operator

    T* pointer(void); // override;

    forward_iterator begin(void);
    forward_iterator end(void);
    reversed_iterator rbegin(void);
    reversed_iterator rend(void);

    const_forward_iterator cbegin(void);
    const_forward_iterator cend(void);
    const_reversed_iterator crbegin(void);
    const_reversed_iterator crend(void);

    void push_back(T item);
    void pop_back(void);

    T& front(void); // override;
    T& back(void); // override;
    T& operator [](int index); // override;

    int size(void);// override;

    template<typename Cont>
    friend class ForwardVectorIterator;
    
    template<typename Cont>
    friend class ReversedVectorIterator;

    template<typename Cont>
    friend class ConstForwardVectorIterator;
    
    template<typename Cont>
    friend class ConstReversedVectorIterator;

    // void print(void) override;
};

template<typename T>
vector<T>::vector()
{
    this->ptr = (T*)calloc(SIZE_OF(T));
}

template<typename T>
vector<T>::vector(vector<T>&& other) 
{
    *this = (const vector<T>&)other;

    other.ptr = NULL;
    other.v_size = 0;
}

template<typename T>
vector<T>::vector (std::initializer_list<T> items) : vector()
{
    int index = 0;
    for(auto it = items.begin(); it != items.end(); it++, index++)
        this->ptr[index] = *it;

    this->v_size = items.size();
}

template<typename T>
vector<T>::~vector()
{
    free(this->ptr);
}

template<typename T>
vector<T>& vector<T>::operator = (std::vector<T>&& other)
{
    if(this == &other)
        return *this;

    *this = (const vector<T>&)other;

    other.ptr = NULL;
    other.v_size = 0;
}

template<typename T>
T* vector<T>::pointer(void)
{
    return this->ptr;
}

template<typename T>
typename vector<T>::forward_iterator vector<T>::begin(void)
{
    return vector<T>::forward_iterator(this->ptr, *this);
}

template<typename T>
typename vector<T>::forward_iterator vector<T>::end(void)
{
    return vector<T>::forward_iterator(this->ptr + this->v_size, *this);
}

template<typename T>
typename vector<T>::reversed_iterator vector<T>::rbegin(void)
{
    return vector<T>::reversed_iterator(this->ptr + this->v_size - 1, *this);
}

template<typename T>
typename vector<T>::reversed_iterator vector<T>::rend(void)
{
    return vector<T>::reversed_iterator(this->ptr - 1, *this);
}

template<typename T>
typename vector<T>::const_forward_iterator vector<T>::cbegin(void)
{
    return vector<T>::const_forward_iterator(this->ptr, *this);
}

template<typename T>
typename vector<T>::const_forward_iterator vector<T>::cend(void)
{
    return vector<T>::const_forward_iterator(this->ptr + this->v_size, *this);
}

template<typename T>
typename vector<T>::const_reversed_iterator vector<T>::crbegin(void)
{
    return vector<T>::const_reversed_iterator(this->ptr + this->v_size - 1, *this);
}

template<typename T>
typename vector<T>::const_reversed_iterator vector<T>::crend(void)
{
    return vector<T>::const_reversed_iterator(this->ptr - 1, *this);
}


template<typename T>
void vector<T>::push_back(T item)
{
    this->ptr = (T*)realloc(this->ptr, SIZE_OF(T) * (this->v_size + 1));
    ptr[this->v_size++] = item;
}

template<typename T>
void vector<T>::pop_back(void)
{
    if(!this->v_size)
        return;
    
    T tmp = *(this->ptr + this->v_size - 1);
    *(this->ptr + this->v_size - 1) = (T)NULL;
    this->ptr = (T*)realloc(this->ptr, SIZE_OF(T) * (--this->v_size));
}

template<typename T>
T& vector<T>::front(void)
{
    return this->ptr[0];
}

template<typename T>
T& vector<T>::back(void)
{
    return this->ptr[this->v_size - 1];
}

template<typename T>
T& vector<T>::operator [](int index)
{

    if(index < 0)
    {
        if(abs(index) > this->v_size) 
            return *ptr;

        return *(this->end() + index);
    }

    else if(index >= this->v_size)
        return *this->end();
    
    return *(this->ptr+index);
}

template<typename T>
int vector<T>::size(void)
{
    return this->v_size;
}

// template<typename T>
// void vector<T>::print(void)
// {
//     if(!this->v_size)
//     {
//         std::cout << "[]" << std::endl;
//         return;
//     }

//     std::cout << "[";

//     auto it = this->begin();
//     for(; it != this->end()-1; it++)
//         std::cout << *it << ", ";
    
//     std::cout << *it;
//     std::cout << "]";
// }

}
