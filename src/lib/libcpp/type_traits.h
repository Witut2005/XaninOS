
#pragma once

namespace std
{

template<bool Cond, typename OnTrue, typename OnFalse>
struct _if {};

template<typename OnTrue, typename OnFalse>
struct _if<true, OnTrue, OnFalse> {
    using type = OnTrue;
};

template<typename OnTrue, typename OnFalse>
struct _if<false, OnTrue, OnFalse> {
    using type = OnFalse;
};


/* https://en.cppreference.com/w/cpp/types/remove_pointer */
template< class T > struct remove_pointer { typedef T type; };
template< class T > struct remove_pointer<T*> { typedef T type; };
template< class T > struct remove_pointer<T* const> { typedef T type; };
template< class T > struct remove_pointer<T* volatile> { typedef T type; };
template< class T > struct remove_pointer<T* const volatile> { typedef T type; };

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

#define is_char_ptr(T) std::is_pointer<T>::value && (sizeof(std::remove_pointer<T>) == sizeof(char))

// #define std::is_same<int, K>(T) true//(sizeof(T) == sizeof(int))
// #define is_short(T) true//(sizeof(T) == sizeof(short))
// #define is_char(T) (sizeof(T) == sizeof(char))

template <typename T, typename K>
struct is_same {
    static constexpr bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

template <typename T, bool const_add = false>
struct ConditionalConst {
    using type = T;
};

template <typename T>
struct ConditionalConst<T, true> {
    using type = const T;
};

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
    ReversedListIterator,

    ForwardUnorderedIterator,
    ReversedUnorderedIterator
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

// constexpr Types type(const std::string& x)
// {
//     return Types::string;
// }

}