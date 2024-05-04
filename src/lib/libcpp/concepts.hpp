
#include <lib/libcpp/type_traits.h>

template <typename T>
concept Pointer = std::is_pointer<T>::value;