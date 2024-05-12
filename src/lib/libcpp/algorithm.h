
#pragma once

#include <stdint.h>
#include <sys/macros.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/limits.hpp>
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
constexpr T& max(T& x, T& y)
{
    return (x > y) ? x : y;
}

template <class T>
constexpr T min(T x, T y)
{
    return (x < y) ? x : y;
}

// template <class T>
// constexpr T& min(T& x, T& y)
// {
//     return (x < y) ? x : y;
// }

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
constexpr void swap(T& x, T& y)
{
    T tmp = x;
    x = y;
    y = tmp;
}


template <typename InputIt>
InputIt find_if(InputIt beg, InputIt end, auto predicate)
{
    for (; beg != end; beg++) {
        if (predicate(*beg)) return beg;
    }
    return end;
}

//TODO sus function 
template <typename Cont>
Cont::iterator find_if(Cont& container, auto predicate)
{
    for (auto beg = container.begin(); beg != container.end(); beg++) {
        if (predicate(*beg)) return beg;
    }
    return container.end();
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
    // CHECK IF ARRAY ITERATORS
    for (; beg != end; beg++)
    {
        if (!beg.valid())
            break;
        return beg;
    }
    return InputIt(NULL, beg.container);
}

template<typename Ptr>
struct FindFirst
{
    static Ptr call(Ptr beg, Ptr end, auto finder)
    {
        for (; beg != end; beg++)
        {
            if (!beg.valid())
                break;
            if (finder(beg))
                return beg;
        }
        return InputIt(NULL, beg.container);
    }
};

template<typename Ptr>
struct FindFirst<Ptr*>
{
    static Ptr* call(Ptr* beg, Ptr* end, auto finder)
    {
        if (beg > end) return nullptr;
        for (; beg != end; beg++)
        {
            if (finder(beg))
                return beg;
        }
        return nullptr;
    }
};
// template<typename Ptr>
// struct FindFirst<Ptr**> : FindFirst<Ptr*> {};

template <class T>
constexpr std::pair<T&&, T&&> minmax(T&& a, T&& b)
{
    if (a < b)
        return std::pair<T&&, T&&>(a, b);
    else
        return std::pair<T&&, T&&>(a, b);
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
T** matrix90_rotate_left(T** matrix, uint32_t x, uint32_t y)
{
    T** tmp = (T**)calloc(sizeof(T*) * x);

    for (int i = 0; i < x; i++)
        tmp[i] = (T*)calloc(sizeof(T) * y);

    for (std::pair<uint32_t, uint32_t> x_vals(0, x - 1); x_vals.first < x; x_vals.first++, x_vals.second--)
    {
        for (int i = 0; i < y; i++)
            tmp[x_vals.first][i] = matrix[i][x_vals.second];
    }

    return tmp;
}

template <class T>
T** matrix90_rotate_right(T** matrix, uint32_t x, uint32_t y)
{
    T** tmp = (T**)calloc(sizeof(T*) * x);

    for (int i = 0; i < x; i++)
        tmp[i] = (T*)calloc(sizeof(T) * y);

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

template <typename InputIt, typename OutIt>
OutIt unique_copy(InputIt beg, InputIt end, OutIt inserter, auto predicate)
{
    auto first_arg = beg;
    auto second_arg = ++beg;

    *inserter = *first_arg;
    for (; second_arg != end; second_arg++)
    {
        if (bool result = predicate(*first_arg, *second_arg); !result)
        {
            *(++inserter) = *second_arg;
            first_arg = second_arg;
        }
    }

    return ++inserter;
}

enum class OverflowCheck {
    Addition,
    Subtraction
};

template<OverflowCheck Operation, typename T>
bool is_overflow(T a, T b)
{
    if constexpr (Operation == OverflowCheck::Addition) {
        return ((b > 0 && a > std::numeric_limits<T>::max() - b) ||
            (b < 0 && a < std::numeric_limits<T>::min() - b));
    }
    else {
        return ((b > 0 && a < std::numeric_limits<T>::min() + b) ||
            (b < 0 && a > std::numeric_limits<T>::max() + b));
    }
}

template <typename T>
T get_and_set(T& val, T set)
{
    auto tmp = val;
    val = set;
    return tmp;
};

template<typename T>
constexpr bool bclamp(const T& v, const T& lo, const T& hi)
{
    return v >= lo && v <= hi;
}

//TODO check if can be faster
template <typename T>
bool have_intersection(std::pair<T, T> v1, std::pair<T, T> v2) // pair.first must be smaller than pair.second
{
    return bclamp(v1.first, v2.first, v2.second) || bclamp(v1.second, v2.first, v2.second) ||
        bclamp(v2.first, v1.first, v1.second) || bclamp(v2.second, v1.first, v1.second);
}

}
