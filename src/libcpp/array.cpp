
#include <stdarg.h>
#include <libcpp/initializer_list.hpp>

namespace std
{

template <class T, int X>
class array
{

    private:
    T arr[X];



    public:
    array(array& arr)
    {
        for(int i = 0; i < X; i++)
            this->arr[i] = arr[i];  
    }

    array(std::initializer_list<T> a)
    {
        
        auto it = a.begin();

        for(int i = 0; i < X;i++) 
        {
            arr[i] = it[i];
        }
    }

    T* begin()
    {
        return &arr[0];
    }

    T* end()
    {
        return &arr[X];
    }

    T& operator[](uint32_t index)
    {
        return arr[index];
    }




};


}
