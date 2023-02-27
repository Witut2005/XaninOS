
#pragma once

namespace std
{

/* https://en.cppreference.com/w/cpp/types/remove_pointer */
template< class T > struct remove_pointer                    {typedef T type;};
template< class T > struct remove_pointer<T*>                {typedef T type;};
template< class T > struct remove_pointer<T* const>          {typedef T type;};
template< class T > struct remove_pointer<T* volatile>       {typedef T type;};
template< class T > struct remove_pointer<T* const volatile> {typedef T type;};

template<class T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant; // using injected-class-name
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } // since c++14
};

using true_type = std::integral_constant<bool, true>;
using false_type = std::integral_constant<bool, false>;

template<class T>
struct is_pointer : std::false_type {};
 
template<class T>
struct is_pointer<T*> : std::true_type {};
 
template<class T>
struct is_pointer<T* const> : std::true_type {};
 
template<class T>
struct is_pointer<T* volatile> : std::true_type {};
 
template<class T>
struct is_pointer<T* const volatile> : std::true_type {};


}