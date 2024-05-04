
#pragma once

#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/new.hpp>
#include "./iterator.hpp"

// #ifdef KERNEL_MODULE
// #include <sys/pmmngr/alloc.h>
// #define VECTOR_ALLOC(p) kcalloc(p)
// #define VECTOR_REALLOC(p, s) krealloc(p, s)
// #define VECTOR_FREE(p) kfree(p)
// #else
// #define VECTOR_ALLOC(p) calloc(p)
// #define VECTOR_REALLOC(p, s) realloc(p, s)
// #define VECTOR_FREE(p) free(p)
// #endif

namespace std
{

template <typename T>
class vector
{

public:
    using value_type = T;

    using iterator = RandomAccessIterator<vector<T>>;
    using const_iterator = ConstRandomAccessIterator<vector<T>>;
    using reversed_iterator = RandomAccessReversedIterator<vector<T>>;
    using const_reversed_iterator = ConstRandomAccessReversedIterator<vector<T>>;

    vector(void);
    vector(const vector<T>& other);
    vector(vector<T>&& other);
    vector(initializer_list<T> items);
    ~vector(void);

    constexpr T* data(void) { return m_ptr; }
    constexpr const T* data(void) const { return m_ptr; }

    template <typename InputIt>
    vector(InputIt beg, InputIt end) : vector()
    {
        if (beg >= end) return;

        for (; beg != end; beg++)
            push_back(*beg);
    }

    constexpr int index_serialize(int index) const { return index < 0 ? (int)size() + index : index; }

    constexpr bool valid(void) const { return m_ptr != nullptr; }
    constexpr bool empty(void) const { return m_size > 0; }

    void clear(void);
    void push_back(const T& item);
    void pop_back(void);

    T& front(void) { return *begin(); }
    T& back(void) { return *rbegin(); }
    const T& front(void) const { return *cbegin(); }
    const T& back(void) const { return *crbegin(); }

    constexpr size_t size(void) const { return m_size; }
    constexpr size_t capacity(void) const { return m_capacity; }

    T& operator = (const vector<T>& other);
    T& operator = (vector<T>&& other);

    T& operator[](int index) { return m_ptr[index_serialize(index)]; }
    const T& operator[](int index) const { return m_ptr[index_serialize(index)]; }

    iterator begin(void) { return iterator(*this, 0); }
    iterator end(void) { return iterator(*this, size()); }
    const_iterator cbegin(void) const { return const_iterator(*this, 0); }
    const_iterator cend(void) const { return const_iterator(*this, size()); }

    reversed_iterator rbegin(void) { return reversed_iterator(*this, m_size > 0 ? m_size - 1 : 0); }
    reversed_iterator rend(void) { return reversed_iterator(*this, -1); }
    const_reversed_iterator crbegin(void) const { return const_reversed_iterator(*this, m_size > 0 ? m_size - 1 : 0); }
    const_reversed_iterator crend(void) const { return const_reversed_iterator(*this, -1); }

    static constexpr int npos = -1;

private:
    bool reallocate_if_needed(uint32_t size); // returns true when data was reallocted

    T* m_ptr{ nullptr };
    size_t m_size{ 0 };
    size_t m_capacity{ 1 };

};

template <typename T>
vector<T>::vector()
{
    // m_ptr = (T*)VECTOR_ALLOC(m_capacity * sizeof(T));
    m_ptr = new T[m_capacity];
}

template <typename T>
vector<T>::vector(const vector<T>& other)
{
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    // m_ptr = (T*)VECTOR_ALLOC(m_capacity * sizeof(value_type));
    m_ptr = new T[m_capacity];

    for (int i = 0; i < m_size; i++) {
        m_ptr[i] = other.m_ptr[i];
    }
}

template <typename T>
vector<T>::vector(vector<T>&& other)
{
    m_ptr = other.m_ptr;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_ptr = nullptr;
    other.m_size = other.m_capacity = 0;
}

template <typename T>
vector<T>::vector(initializer_list<T> items) : vector()
{
    for (auto it = items.begin(); it != items.end(); it++) {
        push_back(*it);
    }

    this->m_size = items.size();
}

template <typename T>
vector<T>::~vector(void)
{
    clear();
}

template <typename T>
void vector<T>::clear(void)
{
    if (m_ptr != nullptr)
    {
        for (int i = 0; i < m_size; i++) {
            m_ptr[i].~T();
        }
        free(m_ptr);
    }

    m_size = m_capacity = 0;
    m_ptr = nullptr;
}

template <typename T>
void vector<T>::push_back(const T& item)
{
    reallocate_if_needed(m_size + 1);
    new(&m_ptr[m_size]) T(item);
    m_size++;
}

template <typename T>
void vector<T>::pop_back(void)
{
    if (!m_size) return;

    *rbegin().~T();
    m_size--;
}

template <typename T>
T& vector<T>::operator = (const vector<T>& other)
{
    if (m_ptr != nullptr) {
        clear();
    }

    m_size = other.m_size;
    m_capacity = other.m_capacity;

    m_ptr = new T[other.m_size];
    for (int i = 0; i < other.m_size; i++) {
        push_back(other.m_ptr[i]);
    }
}

template <typename T>
T& vector<T>::operator = (vector<T>&& other)
{
    if (m_ptr != nullptr) {
        clear();
    }

    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_ptr = other.m_ptr;

    other.m_ptr = nullptr;
    other.m_size = other.m_capacity = 0;
}

template <typename T>
bool vector<T>::reallocate_if_needed(uint32_t size)
{
    if (size <= m_capacity) return false;

    m_ptr = (T*)realloc(m_ptr, size * 2 * sizeof(T));
    m_capacity = size * 2;
    return true;

}

} //namespace
