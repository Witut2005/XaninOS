
#pragma once

#include <lib/libcpp/initializer_list.hpp>

namespace std
{


template<class T> struct remove_reference      { typedef T type; };
template<class T> struct remove_reference<T&>  { typedef T type; };
template<class T> struct remove_reference<T&&> { typedef T type; };

//Typedef doesn't support templates. Use "using" keyword
template<class T> 
using remove_reference_t = typename remove_reference<T>::type;
      
template<class T> struct remove_const { typedef T type; };
template<class T> struct remove_const<const T> { typedef T type; };

template< class T >
using remove_const_t = typename remove_const<T>::type;

template< class T > struct remove_const_ref {typedef T type; };
template< class T > struct remove_const_ref<T&> {typedef T type; };
template< class T > struct remove_const_ref<T&&> {typedef T type; };
template< class T > struct remove_const_ref<const T&> {typedef T type; };
template< class T > struct remove_const_ref<const T&&> {typedef T type; };

template< class T >
using remove_const_ref_t = typename remove_const_ref<T>::type;
    
template<class T>
constexpr std::remove_reference_t<T>&& move(T&& obj) noexcept
{
    return static_cast<std::remove_reference_t<T>&&> (obj);
}
    

template<class T, class X>
struct pair
{

    public:
    T first;
    X second;

    pair<T, X>(T x, X y) : first(x), second(y) {}

    pair<T, X>(const pair<T, X>& copy) = default;
    pair<T, X>(pair<T, X>&& copy) {
        this->first = copy.first;
        this->second = copy.second;
    }
    pair<T, X>& operator=(const pair<T, X>& copy) = default;

    pair<T, X>& operator=(pair<T, X>&& copy) {
        this->first = copy.first;
        this->second = copy.second;
    }

    // pair<T, X>& operator = (pair<T, X>& copy) = default;

    bool operator == (const pair<T, X>& other)
    {
        if((this->first == other.first) && (this->second == other.second))
            return true;
        else
            return false;
    }

    bool operator != (const pair<T, X>& other)
    {
        if((this->first == other.first) && (this->second == other.second))
            return false;
        else
            return true;
    }

};


template<class T, class X>
pair<T, X> make_pair(T x, X y)
{
    return {x, y};
}

template<typename T>
constexpr bool is_positive(T value)
{
    return value > 0;
}
template<typename T>

constexpr bool is_negative(T value)
{
    return value < 0;
}

}

