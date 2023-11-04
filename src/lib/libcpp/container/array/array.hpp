
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
        using iterable_type = T *;

        using lreference = T &;
        using rreference = T &&;

        using const_lreference = const T &;
        using const_rreference = const T &&;

        using forward_iterator = ForwardArrayIterator<this_type>;
        using reversed_iterator = ReversedArrayIterator<this_type>;

        using const_forward_iterator = ConstForwardArrayIterator<this_type>;
        using const_reversed_iterator = ConstReversedArrayIterator<this_type>;

        template <class FT, int FSIZE>
        static int find_other_than_default_handler(std::array<FT, FSIZE> arr, FT arg)
        {
            for (int i = 0; i < SIZE; i++)
            {
                if (arr[i] != arg)
                    return i;
            }
            return -1;
        }

        // static int array<T, SIZE>::find_other_than(T key)
        // {
        //     for (int i = 0; i < SIZE; i++)
        //     {
        //         if (ptr[i] != key)
        //             return i;
        //     }
        //     return -1;
        // }

        array();
        array(const array &arr) = default;
        array(std::initializer_list<T> a);

        template <typename InputIt>
        array(InputIt beg, InputIt end, T default_value)
        {
            int i = 0;

            for (; (beg != end) & (i < SIZE); beg++, i++)
                this->ptr[i] = *beg;

            for (; i < SIZE; i++)
                this->ptr[i] = default_value;
        }

        constexpr forward_iterator begin();
        constexpr forward_iterator end();
        constexpr reversed_iterator rbegin();
        constexpr reversed_iterator rend();

        constexpr const_forward_iterator cbegin();
        constexpr const_forward_iterator cend();
        constexpr const_reversed_iterator crbegin();
        constexpr const_reversed_iterator crend();

        T *pointer(void); // override;

        std::array<T, SIZE> &operator=(const std::array<T, SIZE> &other) = default;
        constexpr lreference operator[](const int index);

        constexpr bool valid_element(T &element) const;

        T get_copy(int32_t index) const;
        int find_default_handler(T arg) const;
        int find_other_than_default_handler(T arg) const;
        int find(auto finder);
        int find_other_than(T key);

        T &front(void); // override;
        T &back(void);  // override;

        constexpr int size(void) // override
        {
            return SIZE;
        }

        template <int OTHER_SIZE>
        auto concat(const std::array<T, OTHER_SIZE> &other)
        {
            std::array<T, SIZE + OTHER_SIZE> tmp;

            for (int i = 0; i < SIZE; i++)
                tmp[i] = other.get_copy(i);

            for (int i = SIZE; i < SIZE + OTHER_SIZE; i++)
                tmp[i] = other.get_copy(i - SIZE);

            return tmp;
        }

        template <int TO_SIZE, typename InputIt>
        std::array<T, TO_SIZE> slice(InputIt &&begin)
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
        memset((uint8_t *)this->ptr, 0, sizeof(T) * this->size());
    }

    template <class T, int SIZE>
    array<T, SIZE>::array(std::initializer_list<T> a)
    {
        auto it = a.begin();

        for (int i = 0; i < SIZE; i++)
            this->ptr[i] = it[i];
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
    constexpr T &array<T, SIZE>::operator[](const int index)
    {

        if (index < 0)
        {
            if (index < SIZE * (-1))
                return *(T *)NULL;

            return this->ptr[SIZE + index];
        }

        else if (index > SIZE)
            return *(T *)NULL;

        return this->ptr[index];
    }

    template <class T, int SIZE>
    constexpr bool array<T, SIZE>::valid_element(T &element) const
    {
        return ((uint32_t)&element >= (uint32_t)this->ptr) & ((uint32_t)&element < (uint32_t) & this->ptr[SIZE]);
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

    template <class T, int SIZE>
    int array<T, SIZE>::find(auto finder)
    {
        for (int i = 0; i < SIZE; i++)
        {
            int index = finder();
            if (index != (-1))
                return index;
        }
        return -1;
    }

    template <typename T, int SIZE>
    T *array<T, SIZE>::pointer(void)
    {
        return this->ptr;
    }

    template <typename T, int SIZE>
    T &array<T, SIZE>::front(void)
    {
        return this->ptr[0];
    }

    template <typename T, int SIZE>
    T &array<T, SIZE>::back(void)
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
    std::array<T, SIZE> to_array(T *ptr)
    {
        std::array<T, SIZE> arr;

        for (int i = 0; i < SIZE; i++)
            arr[i] = ptr[i];
    }

}
