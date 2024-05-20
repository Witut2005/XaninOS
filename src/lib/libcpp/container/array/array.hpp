
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/type_traits.h>
#include "./iterator.hpp"

namespace std
{
template <typename T, size_t Size>
class array
{

public:
    using value_type = T;

    using iterator = RandomAccessIterator<array<T, Size>>;
    using const_iterator = ConstRandomAccessIterator<array<T, Size>>;
    using reversed_iterator = RandomAccessReversedIterator<array<T, Size>>;
    using const_reversed_iterator = ConstRandomAccessReversedIterator<array<T, Size>>;

    array(void);
    array(const array<T, Size>& other);
    array(initializer_list<T> items)
    {
        int i = 0;
        for (auto beg = items.begin(); beg != items.end(); beg++) {
            m_array[i] = *beg;
        }
    }

    constexpr T* data(void) { return m_array; }
    constexpr const T* data(void) const { return m_array; }

    template <typename InputIt>
    array(InputIt beg, InputIt end)
    {
        int i = 0;
        if (beg >= end) return;

        for (; beg != end; beg++) {
            m_array[i] = *beg;
        }
    }

    constexpr int index_serialize(int index) const { return index < 0 ? (int)Size + index : index; }

    T& front(void) { return *begin(); }
    T& back(void) { return *rbegin(); }
    const T& front(void) const { return *cbegin(); }
    const T& back(void) const { return *crbegin(); }

    constexpr size_t size(void) const { return Size; }

    array<T, Size> operator = (const array<T, Size>& other);

    T& operator[](int index) { return m_array[index_serialize(index)]; }
    const T& operator[](int index) const { return m_array[index_serialize(index)]; }

    iterator begin(void) { return iterator(*this, 0); }
    iterator end(void) { return iterator(*this, Size); }
    const_iterator cbegin(void) const { return const_iterator(*this, 0); }
    const_iterator cend(void) const { return const_iterator(*this, Size); }

    reversed_iterator rbegin(void) { return reversed_iterator(*this, Size - 1); }
    reversed_iterator rend(void) { return reversed_iterator(*this, -1); }
    const_reversed_iterator crbegin(void) const { return const_reversed_iterator(*this, Size - 1); }
    const_reversed_iterator crend(void) const { return const_reversed_iterator(*this, -1); }

    uint32_t find(const T& value, uint32_t pos = 0) const
    {
        for (int i = pos; i < Size; i++) {
            if (m_array[i] == value) return i;
        }
        return npos;
    }

    uint32_t find_first_not_of(const T& value, uint32_t pos = 0) const
    {
        for (int i = pos; i < Size; i++) {
            if (m_array[i] != value) return i;
        }
        return npos;
    }

    static constexpr int npos = -1;

private:
    T m_array[Size];
};

} // namespace