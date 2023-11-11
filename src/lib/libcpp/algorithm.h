

#pragma once
#include <stdint.h>
#include <lib/libcpp/utility.h>
#include <sys/macros.h>
#include <lib/libcpp/container/vector/vector.hpp>

namespace std
{

    template <class input_type, class f_type>
    f_type for_each(input_type begin, input_type end, f_type function)
    {

        for (input_type i = begin; i != end; i++)
        {
            function(*i);
        }

        return function;
    }

    template <class T>
    constexpr T &max(T &x, T &y)
    {
        return (x > y) ? x : y;
    }

    template <class T>
    constexpr T &min(T &x, T &y)
    {
        return (x < y) ? x : y;
    }

    template <class T>
    constexpr T min_element(T x, T y)
    {
        auto min = x;

        for (auto a = x; a != y; a++)
        {
            if (*a <= *min)
                min = a;
        }

        return min;
    }

    template <class T>
    constexpr T max_element(T x, T y)
    {
        auto max = x;

        for (auto a = x; a != y; a++)
        {
            if (*a >= *max)
                max = a;
        }

        return max;
    }

    template <class T>
    constexpr void swap(T &x, T &y)
    {
        T tmp = x;
        x = y;
        y = tmp;
    }

    // IN ITERATOR MAKE PTR TO CONTAINER OBJECT
    template <typename InputIt>
    std::vector<InputIt> find(InputIt beg, InputIt end, auto finder)
    {
        std::vector<InputIt> results;
        // CHECK IF ARRAY ITERATORS
        int i = 0;
        for (; beg != end; beg++)
        {
            if (!beg.valid())
                break;
            if (finder(beg))
                results.push_back(beg);
        }
        return results;
    }


    template <typename InputIt>
    InputIt find_first(InputIt beg, InputIt end, auto finder)
    {
        std::vector<InputIt> results;
        // CHECK IF ARRAY ITERATORS
        int i = 0;
        for (; beg != end; beg++)
        {
            if (!beg.valid())
                break;
            return beg;
        }
        return InputIt(NULL, beg.container);
    }


    template <class T>
    constexpr std::pair<T &&, T &&> minmax(T &&a, T &&b)
    {
        if (a < b)
            return std::pair<T &&, T &&>(a, b);
        else
            return std::pair<T &&, T &&>(a, b);
    }

    template <class T, class X>
    bool equal(T beg_first, T end_first, X beg_second)
    {
        auto x = beg_first;

        for (; beg_first != end_first; beg_first++, beg_second++)
        {
            if (*beg_first != *beg_second)
                return false;
        }

        return true;
    }

    template <class T, class X>
    bool equal(T x, X t)
    {
        return x == t;
    }

    template <class T>
    T **matrix90_rotate_left(T **matrix, uint32_t x, uint32_t y)
    {
        T **tmp = (T **)calloc(SIZE_OF(T *) * x);

        for (int i = 0; i < x; i++)
            tmp[i] = (T *)calloc(SIZE_OF(T) * y);

        for (std::pair<uint32_t, uint32_t> x_vals(0, x - 1); x_vals.first < x; x_vals.first++, x_vals.second--)
        {
            for (int i = 0; i < y; i++)
                tmp[x_vals.first][i] = matrix[i][x_vals.second];
        }

        return tmp;
    }

    template <class T>
    T **matrix90_rotate_right(T **matrix, uint32_t x, uint32_t y)
    {
        T **tmp = (T **)calloc(SIZE_OF(T *) * x);

        for (int i = 0; i < x; i++)
            tmp[i] = (T *)calloc(SIZE_OF(T) * y);

        for (int i = 0; i < x; i++)
        {
            for (std::pair<int, int> para(0, y - 1); para.first < y; para.first++, para.second--)
                tmp[i][para.first] = matrix[para.second][i];
        }

        return tmp;
    }

    template <class T>
    bool is_in_range(T start, T end, T value)
    {
        if (start > end)
            std::swap(start, end);

        for (auto i = start; i < end; i++)
            if (i == value)
                return true;

        return false;
    }

}
