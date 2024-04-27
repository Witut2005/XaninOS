
#pragma once

#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/type_traits.h>

#include "./iterator.hpp"

#ifdef KERNEL_MODULE
#include <sys/pmmngr/alloc.h>
#define VECTOR_ALLOC(p) kcalloc(p)
#define VECTOR_FREE(p) kfree(p)
#else
#define VECTOR_ALLOC(p) calloc(p)
#define VECTOR_FREE(p) free(p)
#endif

namespace std
{

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
    vector(const vector<T>& other) = default; // copy constructor
    vector(vector<T>&& other);                // move constructor
    vector(std::initializer_list<T> items);

    template <typename InputIt>
    vector(InputIt beg, InputIt end) : vector()
    {
        for (; beg != end; beg++)
            this->push_back(*beg);
    }

    ~vector();

    std::vector<T>& operator=(const vector<T>& other) = default; // copy assigment operator
    std::vector<T>& operator=(vector<T>&& other);                // move assigment operator

    T* pointer(void); // override;

    inline iterable_type begin_ptr()
    {
        return ptr;
    }
    inline iterable_type end_ptr()
    {
        return ptr + this->v_size;
    }
    inline iterable_type rbegin_ptr()
    {
        return ptr + this->v_size - 1;
    }
    inline iterable_type rend_ptr()
    {
        return ptr - 1;
    }

    inline const iterable_type cbegin_ptr()
    {
        return ptr;
    }
    inline const iterable_type cend_ptr()
    {
        return ptr + this->v_size;
    }
    inline const iterable_type crbegin_ptr()
    {
        return ptr + this->v_size - 1;
    }
    inline const iterable_type crend_ptr()
    {
        return ptr - 1;
    }

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

    T& front(void);           // override;
    T& back(void);            // override;
    T& operator[](int index); // override;

    int size(void); // override;

    bool valid_element(T& element) const;

    template <typename Cont>
    friend class ForwardVectorIterator;

    template <typename Cont>
    friend class ReversedVectorIterator;

    template <typename Cont>
    friend class ConstForwardVectorIterator;

    template <typename Cont>
    friend class ConstReversedVectorIterator;

    // void print(void) override;
};

template <typename T>
vector<T>::vector()
{
    this->ptr = (T*)calloc(sizeof(T));
}

template <typename T>
vector<T>::vector(vector<T>&& other)
{
    *this = (const vector<T> &)other;

    other.ptr = NULL;
    other.v_size = 0;
}

template <typename T>
vector<T>::vector(std::initializer_list<T> items) : vector()
{
    int index = 0;
    for (auto it = items.begin(); it != items.end(); it++, index++)
        memcpy((uint8_t*)&this->ptr[index], (uint8_t*)it, sizeof(T));

    this->v_size = items.size();
}

template <typename T>
vector<T>::~vector()
{
    free(this->ptr);
}

template <typename T>
vector<T>& vector<T>::operator=(std::vector<T>&& other)
{
    if (this == &other)
        return *this;

    *this = (const vector<T> &)other;

    other.ptr = NULL;
    other.v_size = 0;
}

template <typename T>
T* vector<T>::pointer(void)
{
    return this->ptr;
}

template <typename T>
typename vector<T>::forward_iterator vector<T>::begin(void)
{
    return vector<T>::forward_iterator(this->ptr, *this);
}

template <typename T>
typename vector<T>::forward_iterator vector<T>::end(void)
{
    return vector<T>::forward_iterator(this->ptr + this->v_size, *this);
}

template <typename T>
typename vector<T>::reversed_iterator vector<T>::rbegin(void)
{
    return vector<T>::reversed_iterator(this->ptr + this->v_size - 1, *this);
}

template <typename T>
typename vector<T>::reversed_iterator vector<T>::rend(void)
{
    return vector<T>::reversed_iterator(this->ptr - 1, *this);
}

template <typename T>
typename vector<T>::const_forward_iterator vector<T>::cbegin(void)
{
    return vector<T>::const_forward_iterator(this->ptr, *this);
}

template <typename T>
typename vector<T>::const_forward_iterator vector<T>::cend(void)
{
    return vector<T>::const_forward_iterator(this->ptr + this->v_size, *this);
}

template <typename T>
typename vector<T>::const_reversed_iterator vector<T>::crbegin(void)
{
    return vector<T>::const_reversed_iterator(this->ptr + this->v_size - 1, *this);
}

template <typename T>
typename vector<T>::const_reversed_iterator vector<T>::crend(void)
{
    return vector<T>::const_reversed_iterator(this->ptr - 1, *this);
}

template <typename T>
void vector<T>::push_back(T item)
{
    this->ptr = (T*)realloc(this->ptr, sizeof(T) * (this->v_size + 1));
    // YOU MUST USE MEMCPY (= operator can be overloaded)
    memcpy((uint8_t*)&ptr[this->v_size++], (uint8_t*)&item, sizeof(T));
}

template <typename T>
void vector<T>::pop_back(void)
{
    if (!this->v_size)
        return;

    this->ptr = (T*)realloc(this->ptr, sizeof(T) * (--this->v_size));
}

template <typename T>
T& vector<T>::front(void)
{
    return this->ptr[0];
}

template <typename T>
T& vector<T>::back(void)
{
    return this->ptr[this->v_size - 1];
}

template <typename T>
T& vector<T>::operator[](int index)
{

    if (index < 0)
    {
        if (abs(index) > this->v_size)
            return *ptr;

        return *(this->end() + index);
    }

    else if (index >= this->v_size)
        return *this->end();

    return *(this->ptr + index);
}

template <typename T>
int vector<T>::size(void)
{
    return this->v_size;
}

// DOESNT WORK + CONSTS
template <typename T>
bool vector<T>::valid_element(T& element) const
{
    return ((uint32_t)&element >= (uint32_t)this->ptr) & ((uint32_t)&element < (uint32_t)&this->ptr[this->v_size]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class nvector
{

public:
    using value_type = T;

    using Iterator = RandomAccessIterator<nvector<T>>;
    using ConstIterator = ConstRandomAccessIterator<nvector<T>>;
    // using ReversedIterator = ReversedNVectorIterator<vector<T>>;
    // using ConstReversedIterator = ConstReversedNVectorIterator<vector<T>>;

    nvector(void);
    nvector(const nvector<T>& other);
    nvector(nvector<T>&& other);                // move constructor
    nvector(initializer_list<T> items);
    ~nvector(void);

    template <typename InputIt>
    nvector(InputIt beg, InputIt end) : nvector()
    {
        for (; beg != end; beg++)
            push_back(*beg);
    }

    int index_serialize(int index) const;
    void push_back(const T& item);
    T pop_back(void);

    T& front(void);
    T& back(void);
    const T& front(void) const;
    const T& back(void) const;

    size_t size(void) const;
    T& operator[](int index);
    const T& operator[] (int index) const;

    Iterator begin(void) { return Iterator(*this, 0); }
    Iterator end(void) { return Iterator(*this, size()); }

    ConstIterator cbegin(void) { return ConstIterator(*this, 0); }
    ConstIterator cend(void) { return ConstIterator(*this, size()); }

    static constexpr int npos = -1;

private:
    T* m_ptr{ nullptr };
    size_t m_size{ 0 };
    size_t m_capacity{ 1 };

};

template <typename T>
nvector<T>::nvector()
{
    m_ptr = (T*)VECTOR_ALLOC(m_capacity * sizeof(T));
}

template <typename T>
nvector<T>::nvector(const nvector<T>& other)
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    m_ptr = (T*)VECTOR_ALLOC(m_capacity * sizeof(value_type));
    memcpy(m_ptr, other.m_ptr, m_size * sizeof(value_type));
}

template <typename T>
nvector<T>::nvector(nvector<T>&& other)
{
    m_ptr = other.m_ptr;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_ptr = nullptr;
    other.m_size = other.m_capacity = 0;
}

template <typename T>
nvector<T>::nvector(initializer_list<T> items) : nvector()
{
    int index = 0;
    for (auto it = items.begin(); it != items.end(); it++, index++)
        memcpy((uint8_t*)&this->ptr[index], (uint8_t*)it, sizeof(T));

    this->v_size = items.size();
}

template <typename T>
nvector<T>::~nvector(void)
{
    if (m_ptr != nullptr) {
        free(m_ptr);
    }
}

template <typename T>
int nvector<T>::index_serialize(int index) const
{
    return index < 0 ? (int)size() + index : index;
}

template <typename T>
void nvector<T>::push_back(const T& item)
{
    m_ptr[m_size++] = item;
}

template <typename T>
T nvector<T>::pop_back(void)
{
    // return std::get_and_set(*rbegin(), '\0');
}

template <typename T>
size_t nvector<T>::size(void) const
{
    return m_size;
}

template <typename T>
T& nvector<T>::operator[](int index)
{
    return m_ptr[index_serialize(index)];
}

template <typename T>
const T& nvector<T>::operator[](int index) const
{
    return m_ptr[index_serialize(index)];
}

} //namespace
