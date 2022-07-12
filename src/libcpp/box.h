
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

    // T* begin()
    // {
    //     return first; 
    // }

    // T* end()
    // {
    //     return last;
    // }

    Box(Iterator<T> first, Iterator<T> last) : first(first), last(last){}

    void print(void)
    {
        for(auto a = this->first; a != this->last; a++)
            std::cout << *a << " ";
    }

};

}