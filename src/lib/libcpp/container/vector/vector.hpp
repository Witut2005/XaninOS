
#pragma once

#include <stdint.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/math.h>
#include <lib/libc/memory.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/container/iterator.hpp>

// TODO throws heap full error when using k-like allocation funciotns 
// #ifdef KERNEL_MODULE
#include <sys/pmmngr/alloc.h>
#include <sys/devices/com/com.hpp>
#define VECTOR_ALLOC(s) kcalloc(s)
#define VECTOR_REALLOC(p, s) krealloc(p, s)
#define VECTOR_FREE(p) kfree(p)
// #else
// #include <lib/libcpp/alloc.hpp>
// #define VECTOR_ALLOC(s) calloc(s)
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

    void raw_clear(void); //clear without calling deconstructors
    void clear(void);
    void push_back(const T& item);
    void pop_back(void);

    T& front(void) { return *begin(); }
    T& back(void) { return *rbegin(); }
    const T& front(void) const { return *cbegin(); }
    const T& back(void) const { return *crbegin(); }

    constexpr size_t size(void) const { return m_size; }
    constexpr size_t capacity(void) const { return m_capacity; }

    void resize(size_t count);
    void resize(size_t count, const T& value);
    void reserve(size_t capacity);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    vector<T>& operator = (const vector<T>& other);
    vector<T>& operator = (vector<T>&& other);

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
    m_ptr = (T*)VECTOR_ALLOC(m_capacity * sizeof(T));
}

template <typename T>
vector<T>::vector(const vector<T>& other)
{
    m_capacity = other.m_capacity;
    m_ptr = (T*)VECTOR_ALLOC(other.m_capacity * sizeof(T));

    for (int i = 0; i < other.m_size; i++) {
        push_back(other.m_ptr[i]);
    }
}

template <typename T>
vector<T>::vector(vector<T>&& other)
{
    memcpy(this, &other, sizeof(vector<T>));
    other.raw_clear();
}

template <typename T>
vector<T>::vector(initializer_list<T> items) : vector()
{
    for (auto it = items.begin(); it != items.end(); it++) {
        push_back(*it);
    }
}

template <typename T>
vector<T>::~vector(void)
{
    for (int i = 0; i < m_size; i++) {
        m_ptr[i].~T();
    }
    VECTOR_FREE(m_ptr);
}

template <typename T>
void vector<T>::raw_clear(void)
{
    m_size = 0;
    m_capacity = 1;
    m_ptr = (T*)VECTOR_ALLOC(sizeof(T) * m_capacity);
}

template <typename T>
void vector<T>::clear(void)
{
    for (int i = 0; i < m_size; i++) {
        m_ptr[i].~T();
    }
    VECTOR_FREE(m_ptr);

    m_size = 0;
    m_capacity = 1;
    m_ptr = (T*)VECTOR_ALLOC(sizeof(T) * m_capacity);
}

template <typename T>
void vector<T>::push_back(const T& item)
{
    reallocate_if_needed(m_size + 1);
    new(&m_ptr[m_size++]) T(item);
}

template <typename T>
void vector<T>::pop_back(void)
{
    if (!m_size) return;
    m_ptr[m_size--].~T();
}

template <typename T>
void vector<T>::resize(size_t count)
{
    resize(count, T());
}

template <typename T>
void vector<T>::resize(size_t count, const T& value)
{
    if (count > m_size)
    {
        for (int i = 0; i < count - m_size; i++) {
            push_back(value);
        }
    }
    else
    {
        for (int i = 0; i < m_size - count; i++) {
            pop_back();
        }
    }
}

template <typename T>
void vector<T>::reserve(size_t capacity)
{
    if (capacity > m_capacity)
    {
        m_ptr = (T*)VECTOR_REALLOC(m_ptr, capacity * sizeof(T));
        m_capacity = capacity;
    }
}

template <typename T>
vector<T>::iterator vector<T>::erase(vector<T>::iterator pos)
{
    if (pos > end() || pos < begin()) return pos;
    const auto pos_index = pos.index();

    m_ptr[pos_index].~T();
    m_size--;

    for (int i = pos_index; i < m_size; i++) {
        memcpy(&m_ptr[i], &m_ptr[i + 1], sizeof(T));
    }

    return end();
}


template <typename T>
vector<T>::iterator vector<T>::erase(vector<T>::iterator first, vector<T>::iterator last)
{
    if (first >= last)return last;
    if (last > end()) last = end();
    if (first < begin()) first = begin();

    for (int i = 0; i < last.index() - first.index(); i++)
    {
        m_ptr[first.index() + i].~T();
        memcpy(&m_ptr[first.index() + i], &m_ptr[last.index() + i], sizeof(T));
    }

    m_size = m_size - (last.index() - first.index());
    return end();
}

template <typename T>
vector<T>& vector<T>::operator = (const vector<T>& other)
{
    clear();
    m_capacity = other.m_capacity;
    m_ptr = VECTOR_REALLOC(m_ptr, other.m_capacity * sizeof(T));

    for (int i = 0; i < other.m_size; i++) {
        push_back(other.m_ptr[i]);
    }
}

template <typename T>
vector<T>& vector<T>::operator = (vector<T>&& other)
{
    clear();
    memcpy(this, &other, sizeof(vector<T>));

    other.raw_clear();
}

template <typename T>
bool vector<T>::reallocate_if_needed(uint32_t size)
{
    if (m_ptr == nullptr) dbg_error("VECTOR", "M_PTR cannot be null");
    if (size <= m_capacity) return false;

    m_ptr = (T*)VECTOR_REALLOC(m_ptr, size * 2 * sizeof(T));
    m_capacity = size * 2;
    return true;
}

} //namespace