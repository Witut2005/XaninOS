
#include <libcpp/array.h>
#include <libcpp/string.h>
#include <libcpp/iterator.h>

namespace std
{

template <class T>
class Box
{

    Iterator<T> first;
    Iterator<T> last;

    public:

    auto begin()
    {
        return first; 
    }

    auto end()
    {
        return last;
    }

    Box(Iterator<T> first, Iterator<T> last) : first(first), last(last){}


};

}