
#include <lib/libcpp/array.h>
#include <lib/libcpp/string.h>

namespace std
{

template <typename T>
class BoxIterator
{
    
};

template <typename T>
class Box
{

    BoxIterator<T> first;
    BoxIterator<T> last;

    public:

    auto begin()
    {
        return first; 
    }

    auto end()
    {
        return last;
    }

    // Box(<T> first, Iterator<T> last) : first(first), last(last){}


};

}