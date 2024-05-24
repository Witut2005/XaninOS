
#pragma once

#include <stdarg.h>
#include <lib/libcpp/ostream.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/concepts.hpp>
#include <lib/libcpp/type_traits.h>
#include <lib/libcpp/initializer_list.hpp>

namespace std
{
template <typename T, NotZero Size>
class array
{

public:
    using value_type = T;

    using iterator = RandomAccessIterator<array<T, Size>>;
    using const_iterator = ConstRandomAccessIterator<array<T, Size>>;
    using reversed_iterator = RandomAccessReversedIterator<array<T, Size>>;
    using const_reversed_iterator = ConstRandomAccessReversedIterator<array<T, Size>>;

    array(void) = default;
    array(const T& value) { for(int i = 0; i < Size; i++) {m_array[i] = value;}}
    array(const array<T, Size>& other) = default;
    array(const initializer_list<T>& items)
    {
        // static_assert(Size >= items.size(), "initializer list size cannot be larger than array size");
        T* m_ptr = m_array;
        for (auto beg = items.begin(); beg != items.end(); beg++, m_ptr++) {
            *m_ptr = *beg;
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
    constexpr size_t max_size(void) const { return Size; }

    void fill(const T& value) {for(int i = 0; i < Size; i++) {m_array[i] = value;}}
    void swap(array<T, Size>& other) {
        std::array<T, Size> tmp = other;

        for(int i = 0; i < Size; i++){
            other.m_array[i] = m_array[i];
            m_array[i] = tmp[i];
        }
    }

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

    array<T, Size> operator = (const array<T, Size>& other);
    bool operator ==(const array<T, Size>& other) 
    {
        for(int i = 0; i < Size; i++) {
            if(m_array[i] != other.m_array[i]) return false;
        }
        return true;
    }

    bool operator !=(const array<T, Size>& other) 
    {
        return !(*this == other);
    }


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

    static constexpr int npos = -1;

private:
    T m_array[Size];
};



} // namespace