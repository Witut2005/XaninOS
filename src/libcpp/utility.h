
#pragma once


namespace std
{


template<class T> struct remove_reference      { typedef T type; };
template<class T> struct remove_reference<T&>  { typedef T type; };
template<class T> struct remove_reference<T&&> { typedef T type; };

//Typedef doesn't support templates. Use "using" keyword
template<class T> using remove_reference_t = remove_reference<T>::type;
      

    
template<class T>
constexpr std::remove_reference_t<T>&& move(T&& obj) noexcept
{
    return static_cast<std::remove_reference_t<T>&&> (obj);
}
    

}

