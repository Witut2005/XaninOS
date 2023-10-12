
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

#define is_char_ptr(T) std::is_pointer<T>::value && (SIZE_OF(std::remove_pointer<T>) == SIZE_OF(char))

#define is_int(T) (SIZE_OF(T) == SIZE_OF(int))
#define is_short(T) (SIZE_OF(T) == SIZE_OF(short))
#define is_char(T) (SIZE_OF(T) == SIZE_OF(char))

// static const char* const nullptr;

enum class Types
{
    uint8_t, 
    character, 
    uint16_t,
    int16_t, 
    uint32_t,
    integer,
    string,

    ForwardArrayIterator,
    ReversedArrayIterator,

    ForwardVectorIterator,
    ReversedVectorIterator,

    ForwardListIterator,
    ReversedListIterator

};

constexpr Types type(const uint8_t x)
{
    return Types::uint8_t;
}
constexpr Types type(const char x)
{
    return Types::character;
}
constexpr Types type(const uint16_t x)
{
    return Types::uint16_t;
}
constexpr Types type(const int16_t x)
{
    return Types::int16_t;
}
constexpr Types type(const int x)
{
    return Types::integer;
}

constexpr Types type(const std::string& x)
{
    return Types::string;
}

}