
#include <libcpp/list.h>
#include <libcpp/initializer_list.hpp>

template <class T>
class set
{
    private:
    std::List<T> ListItem;
    
    public:

    set(std::initializer_list<T> items) : ListItem(items)
    {
        //ListItem. 
    }

};
