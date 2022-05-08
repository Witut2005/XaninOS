
#pragma once


namespace std
{


template< class T > struct remove_reference      { typedef T type; };
template< class T > struct remove_reference<T&>  { typedef T type; };
template< class T > struct remove_reference<T&&> { typedef T type; };

    template<class T>
    constexpr std::remove_reference<T>&& move(T&& obj)
    {
        return std::remove_reference<T>&&(obj);
    }

}

