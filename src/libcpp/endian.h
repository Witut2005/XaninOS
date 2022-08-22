

namespace endian
{

    template <class T>
    auto switch_to_big(T value) -> decltype(value);
    
    template <class T>
    auto switch_to_little(T value) -> decltype(value);

}