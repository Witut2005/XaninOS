
#include <lib/libcpp/type_traits.h>

template <typename T>
concept Pointer = std::is_pointer<T>::value;

template <size_t T>
concept NotZero =  ( T != 0);